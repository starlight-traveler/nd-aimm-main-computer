#ifndef STATUS_LOGGER_H
#define STATUS_LOGGER_H

#include <mutex>
#include <string>
#include <map>
#include <tuple>

class StatusLogger {
public:
    static StatusLogger* getInstance();

    void updateStatus(const std::string &category, const std::string &testString, int passes, int warnings, int errors, const std::string &operationStatus);
    void incrementPasses(const std::string &category, const std::string &testString);
    void incrementWarnings(const std::string &category, const std::string &testString);
    void incrementErrors(const std::string &category, const std::string &testString);
    std::tuple<int, int, int, std::string> getStatus(const std::string &category);


private:
    StatusLogger() {}
    static StatusLogger* instance;
    static std::mutex mutex;

    std::map<std::string, std::map<std::string, std::tuple<int, int, int, std::string>>> statusMap;
};

#endif // STATUS_LOGGER_H
