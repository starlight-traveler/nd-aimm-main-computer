#include <dexode/EventBus.hpp>
#include "emergency.h"

void emergency_instance(quill::Logger* logger, const std::shared_ptr<EmergencyListener>& listener) {

    while (listener)
    {
        // Processing inside the listener itself, or just sleeping here if nothing is needed
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void shutdown(quill::Logger* logger) {

    LOG_INFO(logger, "System is shutting down.");
    
}