#pragma once

#include <dexode/EventBus.hpp>
#include <memory>
#include <atomic>
#include <thread>

extern std::atomic<bool> running;

void event_processor(const std::shared_ptr<dexode::EventBus> &bus);