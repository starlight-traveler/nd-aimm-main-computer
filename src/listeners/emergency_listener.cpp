#include "emergency.h"
#include <iostream>

extern std::atomic<bool> running;

// Constructor implementation
EmergencyListener::EmergencyListener(quill::Logger* logger, const std::shared_ptr<dexode::EventBus>& bus)
    : _logger(logger), _bus(bus) {
    _listener = std::make_unique<dexode::EventBus::Listener>(bus);
    setupListener();
}

// Setup listener implementation
void EmergencyListener::setupListener() {
    _listener->listen<event::EmergencyShutoff>(
        [this](const auto&) {
            LOG_INFO(_logger, "Emergency shutoff detected!");
            shutdown(_logger);
            running = false;
        }
    );
}