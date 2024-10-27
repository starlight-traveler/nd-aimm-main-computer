// Include the necessary headers
#include "mavlink_dameon.h"
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <iostream>
#include <chrono>
#include <thread>

// Use the mavsdk and standard namespaces
using namespace mavsdk;

void mavlink_dameon(quill::Logger *logger)
{
    // Create a Mavsdk instance
    Mavsdk mavsdk;

    // Set the connection URL for your Pixhawk
    std::string connection_url = "serial:///dev/ttyUSB0:57600";
    // For Windows, you might use: "serial://COM3:115200"
    // For UDP connection: "udp://:14540"

    // Attempt to establish a connection
    ConnectionResult connection_result = mavsdk.add_any_connection(connection_url);
    if (connection_result != ConnectionResult::Success)
    {
        LOG_ERROR(logger, "Connection failed: {}", connection_result_str(connection_result));
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

        if (system->has_autopilot()) {
            LOG_INFO(logger, "Discovered autopilot");
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

    // Subscribe to heartbeat messages
    telemetry.subscribe_heartbeat([logger](Telemetry::Heartbeat heartbeat)
                                  { LOG_INFO(logger, "Received heartbeat: base_mode={}, custom_mode={}, system_status={}",
                                                 static_cast<int>(heartbeat.base_mode),
                                                 heartbeat.custom_mode,
                                                 static_cast<int>(heartbeat.system_status)); });

    // Keep the program running to receive heartbeats
    LOG_INFO(logger, "Started receiving heartbeats...");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    LOG_INFO(logger, "Finished receiving heartbeats.");
}
