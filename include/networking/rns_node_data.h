#ifndef RNS_NODE_DATA_H
#define RNS_NODE_DATA_H

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include "logger.h"

// Structure for node data
struct RNSData
{
    std::string name;
    std::string hexID;
    bool online;
    std::string routingID;
};

// Extern declarations for global variables
extern std::map<std::string, RNSData> nodeDataMap;
extern std::mutex mapMutex;

// Function declarations
void initializeNodes();
RNSData getRNSData(const std::string &nodeName);
std::string getRNSHexID(const std::string &nodeName);
void printNodeStatuses(quill::Logger *logger);

#endif // RNS_NODE_DATA_H
