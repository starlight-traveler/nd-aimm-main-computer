#pragma once
#include <vector>
#include <string>
#include <mutex>
#include <algorithm>

#include "event_listener.h"

class EventBus {
public:
    void registerListener(EventListener *listener) {
        std::lock_guard<std::mutex> lock(mutex_);
        listeners_.push_back(listener);
    }

    void unregisterListener(EventListener *listener) {
        std::lock_guard<std::mutex> lock(mutex_);
        listeners_.erase(
            std::remove(listeners_.begin(), listeners_.end(), listener),
            listeners_.end());
    }

    void publish(const std::string &event) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto *listener : listeners_) {
            if (listener->shouldListen(event)) {
                listener->onEvent(event);
            }
        }
    }

private:
    std::vector<EventListener*> listeners_;
    std::mutex mutex_;
};