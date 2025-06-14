//
// Created by Christian Caamano on 6/6/25.
//

#include "fedex.h"

FedEx::FedEx(QObject *parent)
    : QObject(parent)
{

}

void FedEx::processRawData(const QByteArray& rawData, const QString& identifier, const bool authenticated)
{
    // Process data - collect complete frame, sanitize, validate, parse to Message

    // STILL TODO: sanitize & validate somewhere in this chain!

    auto& buffer = sourceBuffers[identifier]; // Creates if doesn't exist

    // Append new data to this source's buffer
    buffer.recBuffer.append(rawData);

    // Process all complete messages from this buffer
    while (processNextMessage(buffer, identifier, authenticated)) {
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
        qDebug() << "FedEx: shipped " << bytesWritten << "bytes to " << socket->peerAddress();
    }
}

bool FedEx::processNextMessage(SourceBuffer& sourceBuffer, const QString& identifier, const bool authenticated)
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


    // Parse protobuf and convert to Message
    Message message = parseProtoToMessage(protoData);
    emit messageReady(message, identifier, authenticated);

    if (message.getPayload().empty()) {
        qWarning() << "FedEx: empty payload after parsing proto to message";
    }

    qDebug("Fedex successfully parsed proto to message and emit messageReady.");
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

    // Debug what we're actually receiving
    std::string payloadStr = protoMessage.payload().json_data();
    qDebug() << "FedEx: Raw protobuf data size:" << protoData.size();
    qDebug() << "FedEx: Protobuf message type:" << protoMessage.type();
    qDebug() << "FedEx: Payload string length:" << payloadStr.length();
    qDebug() << "FedEx: Payload content:" << QString::fromStdString(payloadStr);

    qDebug("FedEx parseProtoToMessage: parsing proto paylod to json...");
    try {
        payload = nlohmann::json::parse(protoMessage.payload().json_data());

    } catch (std::exception& e) {
        qDebug("FedEx parseProtoToMessage: error parsing proto paylod to json...");
        qWarning() << e.what();
    }
    qDebug("FedEx parseProtoToMessage: parsing proto payload to json complete.");
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
    headerProto->set_receiver_uuid(header["receiver"]["uuid"].get<std::string>());
    headerProto->set_receiver_host(header["receiver"]["host"].get<std::string>());

    protoMessage.set_type(static_cast<int32_t>(type));

    omniserver::PayloadProto* payloadProto = protoMessage.mutable_payload();
    payloadProto->set_json_data(payload.dump());


    // Check if conversion was successful
    if (!protoMessage.IsInitialized()) {
        throw std::runtime_error("FedEx messageToProto: message conversion to protobuf failed.");
    }

    qDebug("FedEx: successfully converted message to proto");

    // Everything good, return
    return protoMessage;
}
