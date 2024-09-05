#pragma once

#include <dexode/EventBus.hpp>
#include <memory>
#include <atomic>
#include <thread>

extern std::atomic<bool> running;

namespace event {

    // General Events
    struct InitalizationComplete{};
    struct HeartBeat{};
    struct TakeOff{};

    // Safety Events
    struct SafetySystemsComplete{};

    // Error Events
    struct EmergencyShutoff {};
    struct ElectricalPowerError{};
}

void event_processor(const std::shared_ptr<dexode::EventBus> &bus);