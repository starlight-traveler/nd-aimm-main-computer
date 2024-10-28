// Include the necessary headers
#include "mavlink_dameon.h"
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/connection_result.h>
#include <iostream>
#include <chrono>
#include <thread>

// Use the mavsdk namespace
using namespace mavsdk;

void mavlink_dameon(quill::Logger *logger)
{
    // Create a Mavsdk instance with the GroundStation configuration
    Mavsdk mavsdk{Mavsdk::Configuration{Mavsdk::ComponentType::CompanionComputer}};

    // Set the connection URL for your Pixhawk
    std::string connection_url = "serial:///dev/tty.usbmodem2101:115200";
    // For Windows, you might use: "serial://COM3:115200"
    // For UDP connection: "udp://:14540"

    // Attempt to establish a connection
    ConnectionResult connection_result = mavsdk.add_any_connection(connection_url);
    if (connection_result != ConnectionResult::Success)
    {
        LOG_ERROR(logger, "Connection failed");
        return;
    }

    LOG_INFO(logger, "Waiting to discover system...");

    // Flag to check if the system has been discovered
    bool discovered_system = false;
    std::shared_ptr<System> system;

    // Subscribe to new system discovery
    mavsdk.subscribe_on_new_system([&]()
                                   {
        auto systems = mavsdk.systems();
        system = systems.back();

        if (system->is_connected()) {
            LOG_INFO(logger, "Discovered system");
            discovered_system = true;
        } });

    // Wait for system discovery with a timeout
    const auto timeout = std::chrono::seconds(10);
    auto start_time = std::chrono::steady_clock::now();
    while (!discovered_system &&
           std::chrono::steady_clock::now() - start_time < timeout)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!discovered_system)
    {
        LOG_ERROR(logger, "No system found, exiting.");
        return;
    }

    // Create a Telemetry plugin instance
    Telemetry telemetry{system};

    // Subscribe to armed messages as a substitute for heartbeats
    telemetry.subscribe_armed([logger](bool armed)
                              { LOG_INFO(logger, "Vehicle armed state changed: {}", armed ? "ARMED" : "DISARMED"); });

    // Keep the program running to receive telemetry
    LOG_INFO(logger, "Started receiving telemetry...");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    LOG_INFO(logger, "Finished receiving telemetry.");
}
