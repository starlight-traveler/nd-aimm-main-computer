#include "events.h"

std::atomic<bool> running;

using EventBus = dexode::EventBus;
using Listener = dexode::EventBus::Listener;

void event_processor(const std::shared_ptr<EventBus> &bus)
{
    while (true)
    {
        bus->process();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}