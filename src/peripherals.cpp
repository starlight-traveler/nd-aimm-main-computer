#include "logger.h"
#include "udp.h"
#include <map>

auto buildPeripherals(quill::Logger *logger) -> std::map<std::string, UDPClient>
{
    std::map<std::string, UDPClient> clients;
    try
    {
        clients.emplace("eth0", UDPClient("Name", "192.168.1.1", 12345, logger));
        LOG_TRACE_L1(logger, "UDPClient for eth0 initialized successfully.");
    }
    catch (const std::exception &e)
    {
        LOG_ERROR(logger, "Failed to initialize UDPClient for eth0: {}", e.what());
    }
    return clients;
}