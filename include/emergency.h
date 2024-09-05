#ifndef SHUTDOWN_H
#define SHUTDOWN_H

#include "emergency_listener.h"
#include "logger.h"



void emergency_instance(quill::Logger* logger, const std::shared_ptr<EmergencyListener>& listener);
void shutdown(quill::Logger* logger);

#endif // SHUTDOWN_H
