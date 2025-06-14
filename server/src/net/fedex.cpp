//
// Created by Christian Caamano on 6/6/25.
//

#include "fedex.h"

FedEx::FedEx(QObject *parent)
    : QObject(parent)
{

}

void FedEx::processRawData(const QByteArray& rawData, const QString& identifier)
{
    // Process data - collect complete frame, sanitize, validate, parse to Message

    auto& buffer = sourceBuffers[identifier]; // Creates if doesn't exist

    // Append new data to this source's buffer
    buffer.recBuffer.append(rawData);

    // Process all complete messages from this buffer
    while (processNextMessage(buffer, identifier)) {
        // Keep processing until no complete messages remain
    }
}

void FedEx::shipMessage(Message* message, QSslSocket* socket)
{
    if (!message || !socket) {
        throw std::runtime_error("FedEx shipMessage: null message or socket");
    }

    // Convert message to protobuf
    omniserver::MessageProto protoMessage = messageToProto(*message);

    // Serialize to bytes with automatic length delimiter
    std::string serializedData;
    google::protobuf::io::StringOutputStream output(&serializedData);
    google::protobuf::io::CodedOutputStream coded_output(&output);

    // Write delimited message (automatically includes length prefix)
    coded_output.WriteVarint32(protoMessage.ByteSizeLong());
    protoMessage.SerializeToCodedStream(&coded_output);

    // Send data over socket
    QByteArray dataToSend = QByteArray::fromStdString(serializedData);
    qint64 bytesWritten = socket->write(dataToSend);

    if (bytesWritten == -1) {
        throw std::runtime_error("FedEx shipMessage: failed to write message to socket.");
    } else {
        qDebug() << "FedEx: shipped " << bytesWritten << " to " << socket->peerAddress();
    }
}

bool FedEx::processNextMessage(SourceBuffer& sourceBuffer, const QString& identifier)
{
    // If we don't know the message size yet, try to read the varint32 length
    if (sourceBuffer.expectedMessageSize == 0) {
        if (sourceBuffer.recBuffer.size() < 1) {
            return false; // Not enough data for varint
        }

        // Parse varint32 length (protobuf's WriteVarint32 format)
        google::protobuf::io::ArrayInputStream input(sourceBuffer.recBuffer.constData(), sourceBuffer.recBuffer.size());
        google::protobuf::io::CodedInputStream coded_input(&input);

        uint32_t messageSize;
        if (!coded_input.ReadVarint32(&messageSize)) {
            return false; // Incomplete varint
        }

        sourceBuffer.expectedMessageSize = messageSize;

        // Remove the varint bytes from buffer
        int varintBytes = coded_input.CurrentPosition();
        sourceBuffer.recBuffer.remove(0, varintBytes);
    }

    // Check if we have the complete message
    if (sourceBuffer.recBuffer.size() < sourceBuffer.expectedMessageSize) {
        return false; // Not enough data yet
    }

    // Extract the complete protobuf message
    QByteArray protoData = sourceBuffer.recBuffer.left(sourceBuffer.expectedMessageSize);
    sourceBuffer.recBuffer.remove(0, sourceBuffer.expectedMessageSize);

    // Reset for next message
    sourceBuffer.expectedMessageSize = 0;

    try {
        // Parse protobuf and convert to Message
        Message message = parseProtoToMessage(protoData);
        emit messageReady(message, identifier);
    } catch (const std::exception& e) {
        qWarning() << "FedEx: Failed to parse message from" << identifier << ":" << e.what();
        // Continue processing other messages
    }

    return true; // Successfully processed a message
}

Message FedEx::parseProtoToMessage(const QByteArray& protoData)
{
    // Parse proto from completed byte frame
    omniserver::MessageProto protoMessage;
    if (!protoMessage.ParseFromArray(protoData.constData(), protoData.size())) {
        throw std::runtime_error("FedEx parseProtoToMessage: failed to parse protobuf message from completed byte frame");
    }

    // Convert proto to Message
    nlohmann::json header;
    MessageType type;
    nlohmann::json payload;

    header["sender"]["class"] = protoMessage.header().sender_class();
    header["sender"]["uuid"] = protoMessage.header().sender_uuid();
    header["sender"]["host"] = protoMessage.header().sender_host();
    header["receiver"]["class"] = protoMessage.header().receiver_class();
    header["receiver"]["uuid"] = protoMessage.header().receiver_uuid();
    header["receiver"]["host"] = protoMessage.header().receiver_host();

    type = static_cast<MessageType>(protoMessage.type());

    payload = nlohmann::json::parse(protoMessage.payload().json_data());

    return Message(header, type, payload);
}


omniserver::MessageProto FedEx::messageToProto(const Message& message)
{
    // Convert
    omniserver::MessageProto protoMessage;

    nlohmann::json header = message.getHeader();
    MessageType type = message.getType();
    nlohmann::json payload = message.getPayload();

    omniserver::HeaderProto* headerProto = protoMessage.mutable_header();
    headerProto->set_sender_class(header["sender"]["class"].get<std::string>());
    headerProto->set_sender_uuid(header["sender"]["uuid"].get<std::string>());
    headerProto->set_sender_host(header["sender"]["host"].get<std::string>());

    headerProto->set_receiver_class(header["receiver"]["class"].get<std::string>());
    headerProto->set_receiver_class(header["receiver"]["uuid"].get<std::string>());
    headerProto->set_receiver_class(header["receiver"]["host"].get<std::string>());

    protoMessage.set_type(static_cast<int32_t>(type));

    omniserver::PayloadProto* payloadProto = protoMessage.mutable_payload();
    payloadProto->set_json_data(payload.get<std::string>());


    // Check if conversion was successful
    if (!protoMessage.IsInitialized()) {
        throw std::runtime_error("FedEx messageToProto: message conversion to protobuf failed.");
    }


    // Everything good, return
    return protoMessage;
}
