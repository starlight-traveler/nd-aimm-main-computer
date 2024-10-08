#include <iostream>
#include <vector>
#include <mutex>
#include <cstring> // For memcpy
#include "flatbuffers/flatbuffers.h"
#include "heartbeat_generated.h" // Generated FlatBuffers schema

class HeartbeatManager {
private:
    static const size_t BUFFER_SIZE = 10; // Size of the ring buffer
    std::vector<uint8_t*> buffer;
    size_t current_index = 0;
    std::mutex buffer_mutex;

public:
    HeartbeatManager() : buffer(BUFFER_SIZE, nullptr) {
        for (auto &b : buffer) {
            b = new uint8_t[1024]; // Assuming each heartbeat message won't exceed 1024 bytes
        }
    }

    ~HeartbeatManager() {
        for (auto &b : buffer) {
            delete[] b;
        }
    }

    void updateHeartbeat(const uint8_t *data, size_t size) {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        // Copy the new data into the current slot of the ring buffer
        memcpy(buffer[current_index], data, size);
        current_index = (current_index + 1) % BUFFER_SIZE;
    }

    const aimm::heartbeat::Heartbeat* getCurrentHeartbeat() {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        return aimm::heartbeat::GetHeartbeat(buffer[(current_index - 1 + BUFFER_SIZE) % BUFFER_SIZE]);
    }

    // Getter functions
    std::string getTimestamp() {
        auto hb = getCurrentHeartbeat();
        return hb->timestamp()->str();
    }

    float getPumpPressure() {
        auto hb = getCurrentHeartbeat();
        return hb->pump_pressure();
    }

    float getDepth() {
        auto hb = getCurrentHeartbeat();
        return hb->depth();
    }

    std::vector<bool> getValveStates() {
        auto hb = getCurrentHeartbeat();
        const auto &states = *hb->valveStates()->states();
        return std::vector<bool>(states.begin(), states.end());
    }

    float getPressurizationPressure() {
        auto hb = getCurrentHeartbeat();
        return hb->pressurization()->pressure();
    }

    bool isTargetPressure() {
        auto hb = getCurrentHeartbeat();
        return hb->pressurization()->is_target_pressure();
    }

    float getMainBatteryVoltage() {
        auto hb = getCurrentHeartbeat();
        return hb->voltages()->main_battery();
    }


};

// // Placeholder for initialization and usage of HeartbeatManager
//     HeartbeatManager manager;

//     // Example of updating heartbeat
//     // Assume 'serializedData' and 'dataSize' are obtained from serialization or network
//     uint8_t serializedData[1024];
//     size_t dataSize = sizeof(serializedData);
//     manager.updateHeartbeat(serializedData, dataSize);

//     // Reading a value
//     std::cout << "Timestamp: " << manager.getTimestamp() << std::endl;
//     std::cout << "Depth: " << manager.getDepth() << std::endl;