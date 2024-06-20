#ifndef PERIPHERALS_SETUP
#define PERIPHERALS_SETUP

#include <map>
#include "quill/Logger.h"

extern std::map<std::string, UDPClient> buildPeripherals(quill::Logger *logger);

#endif // PERIPHERALS_SETUP
