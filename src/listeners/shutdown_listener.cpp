#include "shutdown_listener.h"

ShutdownListener::ShutdownListener(quill::Logger *logger)
    : logger(logger) {}

bool ShutdownListener::shouldListen(const std::string &event) const
{
    return event == "EMERGENCY_SHUTDOWN";
}

void ShutdownListener::onEvent(const std::string &event)
{
    LOG_INFO(logger, "Emergency shutdown triggered by event: {}", event);
    // Add shutdown logic here
}
