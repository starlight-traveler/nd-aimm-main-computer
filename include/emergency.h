#ifndef SHUTDOWN_H
#define SHUTDOWN_H

#include "logger.h"

namespace event {
    struct EmergencyShutoff {};
}

void shutdown(quill::Logger* logger);

#endif // SHUTDOWN_H
