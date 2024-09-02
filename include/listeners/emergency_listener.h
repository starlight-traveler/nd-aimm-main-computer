#pragma once

#include "emergency.h"    // Includes definitions like `shutdown` and event structures
#include <dexode/EventBus.hpp>
#include <memory>         // For std::unique_ptr
#include "logger.h" // For quill::Logger

// Forward declaration to minimize dependencies
namespace dexode {
    class EventBus;
}

// The EmergencyListener class definition
class EmergencyListener {
public:
    // Constructor
    EmergencyListener(quill::Logger* logger, const std::shared_ptr<dexode::EventBus>& bus);

private:
    quill::Logger* _logger;
    std::shared_ptr<dexode::EventBus> _bus;
    std::unique_ptr<dexode::EventBus::Listener> _listener;

    // Helper method to set up the listener
    void setupListener();
};