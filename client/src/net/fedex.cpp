#include "fedex.h"

FedEx::FedEx(QObject *parent)
    : QObject(parent)
{

}

void FedEx::setPortAuthority(PortAuthority* portAuthorityArg)
{
    portAuthority = portAuthorityArg;
}

void FedEx::processRawData(const QByteArray& rawData)
{
    // Process data - collect complete frame, sanitize, validate, parse to Message

    // STILL TODO: sanitize & validate somewhere in this chain!

    // Append new data to the buffer
    recBuffer.append(rawData);

    // Process all complete messages from the buffer
    while (processNextMessage()) {
        // Keep processing until no complete messages remain
    }
}

void FedEx::shipMessage(Message* message)
{
    if (!message) {
        throw std::runtime_error("FedEx shipMessage: null message");
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

    // Send data
    QByteArray dataToSend = QByteArray::fromStdString(serializedData);
    portAuthority->write(dataToSend);
}

bool FedEx::processNextMessage()
{
    // Define message size limits
    static const uint32_t MAX_MESSAGE_SIZE = 10 * 1024 * 1024; // 10MB max message size
    static const uint32_t MAX_BUFFER_SIZE = 50 * 1024 * 1024;  // 50MB max buffer size (prevents memory exhaustion)

    // Check if buffer is getting too large (potential DoS protection)
    if (recBuffer.size() > MAX_BUFFER_SIZE) {
        qCritical("FedEx processNextMessage: Buffer size exceeded maximum limit");
        throw std::runtime_error("FedEx processNextMessage: buffer size limit exceeded");
    }

    // If we don't know the message size yet, try to read the varint32 length
    if (expectedMessageSize == 0) {
        if (recBuffer.size() < 1) {
            return false; // Not enough data for varint
        }

        // Parse varint32 length (protobuf's WriteVarint32 format)
        google::protobuf::io::ArrayInputStream input(recBuffer.constData(), recBuffer.size());
        google::protobuf::io::CodedInputStream coded_input(&input);

        uint32_t messageSize;
        if (!coded_input.ReadVarint32(&messageSize)) {
            return false; // Incomplete varint
        }

        // Validate message size
        if (messageSize > MAX_MESSAGE_SIZE) {
            qCritical() << "FedEx: Message size" << messageSize << "exceeds maximum limit of" << MAX_MESSAGE_SIZE;
            throw std::runtime_error("FedEx processNextMessage: message size limit exceeded");
        }

        if (messageSize == 0) {
            qWarning() << "FedEx: Received zero-length message";
            throw std::runtime_error("FedEx processNextMessage: zero-length message not allowed");
        }

        expectedMessageSize = messageSize;
        qDebug() << "FedEx: Expecting message of size" << messageSize << "bytes";

        // Remove the varint bytes from buffer
        int varintBytes = coded_input.CurrentPosition();
        recBuffer.remove(0, varintBytes);
    }

    // Check if we have the complete message
    if (recBuffer.size() < expectedMessageSize) {
        return false; // Not enough data yet
    }

    // Extract the complete protobuf message
    QByteArray protoData = recBuffer.left(expectedMessageSize);
    recBuffer.remove(0, expectedMessageSize);

    // Reset for next message
    expectedMessageSize = 0;

    // Parse protobuf and convert to Message
    Message message = parseProtoToMessage(protoData);
    emit messageReady(message);

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
