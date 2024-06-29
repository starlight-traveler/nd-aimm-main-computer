#include "logger.h"
#include "udp.h"
#include <map>

std::map<std::string, UDPClient> buildPeripherals(quill::Logger *logger)
{

    // Why after the emplace does the destructor get called?
    std::map<std::string, UDPClient> clients;
    clients.emplace("eth0", UDPClient("eth0", "192.168.36.112", 3333, logger));
    clients.emplace("eth1", UDPClient("eth1", "192.168.36.113", 3334, logger));

    return clients;
}
