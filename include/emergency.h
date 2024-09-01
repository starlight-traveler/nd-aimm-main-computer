#ifndef SHUTDOWN_H
#define SHUTDOWN_H

#include "event_bus.h"
#include "logger.h"

void shutdown(quill::Logger *logger, EventBus &bus);

#endif // SHUTDOWN_H
