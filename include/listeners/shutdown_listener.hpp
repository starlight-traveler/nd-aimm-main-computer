#ifndef SHUTDOWN_LISTENER_H
#define SHUTDOWN_LISTENER_H

#include "event_bus.hpp"
#include "logger.h"

class ShutdownListener : public EventListener
{
public:
    explicit ShutdownListener(quill::Logger *logger);

    bool shouldListen(const std::string &event) const override;
    void onEvent(const std::string &event) override;

private:
    quill::Logger *logger;
};

#endif // SHUTDOWN_LISTENER_H
