#include "thread_safe_gui_status_logger.hpp"

// Initialize static members
StatusLogger *StatusLogger::instance = nullptr;
std::mutex StatusLogger::mutex;

StatusLogger *StatusLogger::getInstance()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr)
    {
        instance = new StatusLogger();
    }
    return instance;
}

void StatusLogger::updateStatus(const std::string &category, const std::string &testString, int passes, int warnings, int errors, const std::string &operationStatus)
{
    std::lock_guard<std::mutex> lock(mutex);
    statusMap[category][testString] = std::make_tuple(passes, warnings, errors, operationStatus);
}

void StatusLogger::incrementPasses(const std::string &category, const std::string &testString)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto &status = statusMap[category][testString];
    std::get<0>(status)++;   // Increment passes
    std::get<1>(status) = 0; // Reset warnings
    std::get<2>(status) = 0; // Reset errors
}

void StatusLogger::incrementWarnings(const std::string &category, const std::string &testString)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto &status = statusMap[category][testString];
    std::get<1>(status)++;   // Increment warnings
    std::get<0>(status) = 0; // Reset passes
    std::get<2>(status) = 0; // Reset errors
}

void StatusLogger::incrementErrors(const std::string &category, const std::string &testString)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto &status = statusMap[category][testString];
    std::get<2>(status)++;   // Increment errors
    std::get<0>(status) = 0; // Reset passes
    std::get<1>(status) = 0; // Reset warnings
}

std::tuple<int, int, int, std::string> StatusLogger::getStatus(const std::string &category)
{
    std::lock_guard<std::mutex> lock(mutex);
    int totalPasses = 0, totalWarnings = 0, totalErrors = 0;
    std::string mostRecentOperation = "Unknown";

    auto it = statusMap.find(category);
    if (it != statusMap.end())
    {
        for (const auto &pair : it->second)
        {
            totalPasses += std::get<0>(pair.second);
            totalWarnings += std::get<1>(pair.second);
            totalErrors += std::get<2>(pair.second);
            // You may choose to handle operationStatus differently, depending on your use case
            mostRecentOperation = std::get<3>(pair.second); // Last one found (arbitrary)
        }
    }
    return std::make_tuple(totalPasses, totalWarnings, totalErrors, mostRecentOperation);
}
