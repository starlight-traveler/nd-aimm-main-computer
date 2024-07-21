#ifndef A_STAR_TESTING_H
#define A_STAR_TESTING_H

#include "logger.h"
#include "udp.h"                     // Include this if UDPClient is used in the function declarations
#include "flatbuffers/flatbuffers.h" // Required for flatbuffers::FlatBufferBuilder
#include "peripherals.h"
#include "fb_check_t.h"

#include <map>
#include <string>

namespace testing_a_star
{

    void a_star_runner(quill::Logger *logger);

}
#endif // A_STAR_TESTING_H