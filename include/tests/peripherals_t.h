#ifndef PERIPHERAL_TESTING_H
#define PERIPHERAL_TESTING_H

#include "logger.h"
#include "udp.h"                     // Include this if UDPClient is used in the function declarations
#include "flatbuffers/flatbuffers.h" // Required for flatbuffers::FlatBufferBuilder
#include "peripherals.h"
#include "fb_check_t.h"

#include <map>
#include <string>

namespace testing_peripherals
{
    bool peripheralTest(quill::Logger *logger);
    bool testClientInitialization(const std::map<std::string, UDPClient> &clients, const std::string &clientKey, quill::Logger *logger);
    bool testSendValue(UDPClient &client, flatbuffers::FlatBufferBuilder &builder, quill::Logger *logger);
}
#endif // PERIPHERAL_TESTING_H
