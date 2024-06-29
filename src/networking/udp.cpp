#include "udp.h"
#include <cstring>
#include <cerrno>  // For errno
#include <cstring> // For strerror

UDPClient::UDPClient(std::string name, std::string ip, int port, quill::Logger *logger) : name(name), ip_address(ip), port(port)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        LOG_ERROR(logger, "Socket creation failed: {}", strerror(errno));
        throw std::runtime_error("Socket creation failed: " + std::string(strerror(errno)));
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ip_address.c_str());
}

UDPClient::~UDPClient()
{
    std::cerr << "Closed Socket! " << std::endl;

}

uint16_t UDPClient::calculateCRC(const std::string &data)
{
    uint16_t crc = 0xFFFF; // Initial value
    for (unsigned char byte : data)
    {
        crc ^= static_cast<uint16_t>(byte << 8); // Move byte into MSB of 16-bit CRC
        for (unsigned i = 0; i < 8; i++)
        { // Process each bit
            if (crc & 0x8000)
            {
                crc = (crc << 1) ^ 0x1021; // Polynomial
            }
            else
            {
                crc = crc << 1;
            }
        }
    }
    return crc;
}

std::vector<uint8_t> UDPClient::constructMessage(const std::string &message, uint8_t responseFlag, uint8_t auxillaryFlag)
{
    UDPMessage msg;
    msg.header = 0xABCD; // Example header
    msg.payload = message;
    msg.footer = 0xEF01; // Example footer
    msg.responseFlag = responseFlag;
    msg.auxillaryFlag = auxillaryFlag;
    msg.crc = calculateCRC(message); // Calculate CRC based on the payload

    std::vector<uint8_t> buffer;
    buffer.resize(sizeof(msg.header) + message.length() + sizeof(msg.footer) + sizeof(msg.crc) + 2); // +2 for flags

    size_t offset = 0;
    memcpy(buffer.data() + offset, &msg.header, sizeof(msg.header));
    offset += sizeof(msg.header);
    memcpy(buffer.data() + offset, message.data(), message.length());
    offset += message.length();
    memcpy(buffer.data() + offset, &msg.footer, sizeof(msg.footer));
    offset += sizeof(msg.footer);
    buffer[offset++] = msg.responseFlag;
    buffer[offset++] = msg.auxillaryFlag;
    memcpy(buffer.data() + offset, &msg.crc, sizeof(msg.crc));

    return buffer;
}

void UDPClient::sendMessageAsync(const std::string &message, uint8_t responseFlag, uint8_t auxillaryFlag, quill::Logger *logger)
{
    auto buffer = constructMessage(message, responseFlag, auxillaryFlag);
    auto fut = std::make_shared<std::future<void>>(std::async(std::launch::async, [this, buffer, logger]()
                                                              {
        ssize_t sent_bytes = sendto(sockfd, buffer.data(), buffer.size(), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        if (sent_bytes < 0) {
            LOG_ERROR(logger, "Failed to send message asynchronously to {} ({}): {}", name, ip_address, strerror(errno));
        } else {
            LOG_TRACE_L1(logger, "Asynchronously sent message to {} ({})", name, ip_address);
        } }));
    futures.push_back(std::move(fut));
}

void UDPClient::sendMessageAsyncSerialized(const flatbuffers::FlatBufferBuilder &builder, quill::Logger *logger)
{
    auto buffer = builder.GetBufferPointer();
    auto size = builder.GetSize();
    auto fut = std::make_shared<std::future<void>>(std::async(std::launch::async, [this, buffer, size, logger]()
                                                              {
        ssize_t sent_bytes = sendto(sockfd, buffer, size, 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        if (sent_bytes < 0) {
            LOG_ERROR(logger, "Failed to send serialized message asynchronously to {} ({}): {}", name, ip_address, strerror(errno));
        } else {
            LOG_TRACE_L1(logger, "Asynchronously sent serialized message to {} ({})", name, ip_address);
        } }));
    futures.push_back(std::move(fut));
}
