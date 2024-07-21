#include "initializer.h"

/*
ImVec4(0.5, 0.5, 0.5, 1.0) - Gray
ImVec4(1.0, 1.0, 0.0, 1.0) - Yellow
ImVec4(1.0, 0.0, 0.0, 1.0) - Red
ImVec4(0.0, 1.0, 0.0, 1.0) - Green
*/

void primary_initialization() {

    status_logger_initializer();
    test_manager_adder();
    test_manager_initialization();

}

void secondary_initialization() {

}

void tertiary_initialization() {

}

void status_logger_initializer()
{

    /**
     * @brief Get overall status loggers, probably a better way to do this,
     * but better than the alternative
     *
     */

    // Overview
    StatusLogger::getInstance()->updateStatus("overview", "Startup", 0, 0, 0, "Starting");
    StatusLogger::getInstance()->updateStatus("overview", "Runtime", 0, 0, 0, "Starting");
    StatusLogger::getInstance()->updateStatus("overview", "Peripherals", 0, 0, 0, "Starting");
    StatusLogger::getInstance()->updateStatus("overview", "Power", 0, 0, 0, "Starting");
    StatusLogger::getInstance()->updateStatus("overview", "Radio", 0, 0, 0, "Starting");

    // All

    // Peripherals
    StatusLogger::getInstance()->updateStatus("peripherals", "ESP32 #1", 0, 0, 0, "Starting");
    StatusLogger::getInstance()->updateStatus("peripherals", "ESP32 #2", 0, 0, 0, "Starting");
    StatusLogger::getInstance()->updateStatus("peripherals", "ESP32 #3", 0, 0, 0, "Starting");
    StatusLogger::getInstance()->updateStatus("peripherals", "x86 Computer", 0, 0, 0, "Starting");

    // Radio Frequency
    StatusLogger::getInstance()->updateStatus("radio_frequency", "Frequency", 0, 0, 0, "Starting");
    StatusLogger::getInstance()->updateStatus("peripherals", "CR", 0, 0, 0, "Starting");
    StatusLogger::getInstance()->updateStatus("peripherals", "FR", 0, 0, 0, "Starting");
    StatusLogger::getInstance()->updateStatus("peripherals", "Bit Rate", 0, 0, 0, "Starting");

    // Startup
    StatusLogger::getInstance()->updateStatus("startup", "Placeholder", 0, 0, 0, "Starting");

    // Runtime
    StatusLogger::getInstance()->updateStatus("runtime", "PlaceHolder", 0, 0, 0, "Starting");

    // Flatbuffers
    StatusLogger::getInstance()->updateStatus("flatbuffers", "Flatbuffer General Test", 0, 0, 0, "Starting");

    // Power
    StatusLogger::getInstance()->updateStatus("power", "Main Battery #1", 0, 0, 0, "Starting");
    StatusLogger::getInstance()->updateStatus("power", "Main Battery #2", 0, 0, 0, "Starting");
}

void test_manager_adder() {

    /**
     * @brief Create instances based on section
     *
     */

    // Overview
    TestManager::getInstance()->addTest("overview", "Startup", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));
    TestManager::getInstance()->addTest("overview", "Runtime", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));
    TestManager::getInstance()->addTest("overview", "Peripherals", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));
    TestManager::getInstance()->addTest("overview", "Power", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));
    TestManager::getInstance()->addTest("overview", "Radio", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));

    // All
    // This is handled in another case...

    // Peripherals
    TestManager::getInstance()->addTest("peripherals", "ESP32 #1", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));
    TestManager::getInstance()->addTest("peripherals", "ESP32 #2", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));
    TestManager::getInstance()->addTest("peripherals", "ESP32 #3", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));
    TestManager::getInstance()->addTest("peripherals", "x86 Computer", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));

    // Radio Frequency
    TestManager::getInstance()->addTest("radio_frequency", "Frequency", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));
    TestManager::getInstance()->addTest("radio_frequency", "CR", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));
    TestManager::getInstance()->addTest("radio_frequency", "FR", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));
    TestManager::getInstance()->addTest("radio_frequency", "Bit Rate", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));

    // Startup
    TestManager::getInstance()->addTest("startup", "PlaceHolder", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));

    // Runtime
    TestManager::getInstance()->addTest("runtime", "PlaceHolder", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));

    // Flatbuffers
    TestManager::getInstance()->addTest("flatbuffers", "Flatbuffer General Test", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));

    // Power
    TestManager::getInstance()->addTest("power", "Main Battery #1", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));
    TestManager::getInstance()->addTest("power", "Main Battery #2", "Pending", ImVec4(0.5, 0.5, 0.5, 1.0));

}

void test_manager_initialization() {


    // Overview
    TestManager::getInstance()->updateTest("overview", "Startup", 0, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);
    TestManager::getInstance()->updateTest("overview", "Runtime", 1, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);
    TestManager::getInstance()->updateTest("overview", "Peripherals", 2, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);
    TestManager::getInstance()->updateTest("overview", "Power", 3, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);
    TestManager::getInstance()->updateTest("overview", "Radio", 4, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);

    // Peripherals
    TestManager::getInstance()->updateTest("peripherals", "ESP32 #1", 0, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);
    TestManager::getInstance()->updateTest("peripherals", "ESP32 #2", 1, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);
    TestManager::getInstance()->updateTest("peripherals", "ESP32 #3", 2, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);
    TestManager::getInstance()->updateTest("peripherals", "x86 Computer", 3, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);

    // Radio Frequency
    TestManager::getInstance()->updateTest("radio_frequency", "Frequency", 0, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);
    TestManager::getInstance()->updateTest("radio_frequency", "CR", 1, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);
    TestManager::getInstance()->updateTest("radio_frequency", "FR", 2, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);
    TestManager::getInstance()->updateTest("radio_frequency", "Bit Rate", 3, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);

    // Startup
    TestManager::getInstance()->updateTest("startup", "PlaceHolder", 0, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);

    // Runtime
    TestManager::getInstance()->updateTest("runtime", "PlaceHolder", 0, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);

    // Flatbuffers
    TestManager::getInstance()->updateTest("flatbuffers", "Flatbuffer General Test", 0, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);

    // Power
    TestManager::getInstance()->updateTest("power", "Main Battery #1", 0, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);
    TestManager::getInstance()->updateTest("power", "Main Battery #2", 0, "Pending", ImVec4(1.0, 0.8, 0.0, 1.0), true);
}