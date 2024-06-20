#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <future>
#include <vector>

#include "logger.h" // Assuming this header is set up correctly elsewhere
#include "flatbuffers/flatbuffers.h"

#include <cstdint> // For standard integer type definitions
#include <vector>
#include <string>

struct UDPMessage
{
    uint32_t header;
    std::string payload;
    uint32_t footer;
    uint16_t crc;
    uint8_t responseFlag;
    uint8_t auxillaryFlag;
};

class UDPClient
{
private:
    std::string ip_address;
    int port;
    std::string name;
    int sockfd;
    struct sockaddr_in servaddr;
    std::vector<std::shared_ptr<std::future<void>>> futures;

    uint16_t calculateCRC(const std::string &data);
    std::vector<uint8_t> constructMessage(const std::string &message, uint8_t responseFlag, uint8_t auxillaryFlag);

public:
    UDPClient(std::string name, std::string ip, int port, quill::Logger *logger);
    ~UDPClient();
    void sendMessageAsync(const std::string &message, uint8_t responseFlag, uint8_t auxillaryFlag, quill::Logger *logger);
    void sendMessageAsyncSerialized(const flatbuffers::FlatBufferBuilder &builder, quill::Logger *logger);
};

#endif // UDPCLIENT_H
