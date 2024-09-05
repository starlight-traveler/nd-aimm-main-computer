#ifndef RNS_MANAGER_H
#define RNS_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include <mutex>

// Structure to hold data about a Reticulum node
struct RNSData {
    std::string name;
    std::string hexID;
    bool online;
    std::string routingID; // Added routing ID field
};

// Functions to manage and access node data
void initializeNodes();
RNSData getRNSData(const std::string& nodeName);
void printNodeStatuses(quill::Logger *logger);
void verify_connection(const std::string &hexadecimal, quill::Logger *logger);
std::string getRNSHexID(const std::string &nodeName);
void rnsd_dameon(quill::Logger *logger);
void rns_sender_manager(quill::Logger *logger);

#endif // RNS_MANAGER_H