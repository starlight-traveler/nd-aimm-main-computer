#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include <cstring> // For memset
#include <pybind11/embed.h>
#include "subprocess.hpp"
#include "logger.h"
#include "quill/LogMacros.h"
#include "threading.tpp"

namespace py = pybind11;

// Using declarations
using subprocess::CompletedProcess;
using subprocess::PipeOption;
using subprocess::Popen;
using subprocess::RunBuilder;

// Declare global variables before use
std::vector<std::string> rnsNames = {"nd-aimm"};
std::vector<std::string> rnsHexID = {"cb02f08466a9cac03d6523c504ac1879"};
std::vector<std::string> rnsRoutingID = {"10858d74bc5d400beb73d691181ca1e9"};

// Define a structure for node data
struct RNSData
{
    std::string name;
    std::string hexID;
    bool online;
    std::string routingID; // Added routing ID field
};

// Global map to hold node data, and a mutex to protect it
std::map<std::string, RNSData> nodeDataMap;
std::mutex mapMutex;

// Function prototypes
void rnsd_daemon(quill::Logger *logger);
void rns_receiver_manager(quill::Logger *logger);
void rns_sender_manager(quill::Logger *logger);
void verify_connection(const std::string &hexadecimal, quill::Logger *logger);
void initializeNodes();
void printNodeStatuses(quill::Logger *logger);
RNSData getRNSData(const std::string &nodeName);
std::string getRNSHexID(const std::string &nodeName);

// Implementation of rnsd_daemon
void rnsd_daemon(quill::Logger *logger)
{
    // Start the rnsd subprocess with output piped
    Popen popen = RunBuilder({"rnsd"})
                      .cout(PipeOption::pipe)
                      .popen();

    // Buffer to store the output
    char buf[1024];

    // Read from the output pipe until there's no more data
    size_t bytes_read = 0;
    while ((bytes_read = subprocess::pipe_read(popen.cout, buf, sizeof(buf) - 1)) > 0)
    {
        buf[bytes_read] = '\0'; // Null-terminate the string
        LOG_INFO(logger, "RNSD: {}", buf);
        std::memset(buf, 0, sizeof(buf)); // Clear the buffer
    }

    // Close the process and handle cleanup
    // popen.close(); // Uncomment if necessary
}

// Implementation of rns_receiver_manager
void rns_receiver_manager(quill::Logger *logger)
{
    // TODO: Implement receiver logic
}

// Implementation of rns_sender_manager
void rns_sender_manager(quill::Logger *logger)
{
    py::gil_scoped_acquire acquire;

    initializeNodes();         // Initialize the node data
    printNodeStatuses(logger); // Print the status of all nodes

    // Loop through all nodes and verify connections
    {
        std::lock_guard<std::mutex> lock(mapMutex); // Lock the mutex once for the entire operation
        for (auto &pair : nodeDataMap)
        {
            RNSData &node = pair.second;
            try
            {
                verify_connection(node.routingID, logger); // Verify each node's connection
                node.online = true;                        // Update the node's online status if verification is successful
            }
            catch (const std::exception &e)
            {
                LOG_ERROR(logger, "Failed to verify connection for {} : {}", node.name, e.what());
                node.online = false;
            }
        }
    }

    // Optionally, print updated statuses
    printNodeStatuses(logger);
}

// Implementation of verify_connection
void verify_connection(const std::string &hexadecimal, quill::Logger *logger)
{
    try
    {
        py::module_ verifier = py::module_::import("rns_verifier");
        auto rnpath = verifier.attr("run_rnpath");

        // Call Python function
        rnpath(hexadecimal);
    }
    catch (const py::error_already_set &e)
    {
        LOG_ERROR(logger, "Python error: {}", e.what());
        throw;
    }
}

// Initialize the node data map
void initializeNodes()
{
    // Ensure data vectors are of the same length
    if (rnsNames.size() != rnsHexID.size() || rnsNames.size() != rnsRoutingID.size())
    {
        std::cerr << "Error: Data vectors must be of the same length." << std::endl;
        return;
    }

    std::lock_guard<std::mutex> lock(mapMutex);
    for (size_t i = 0; i < rnsNames.size(); ++i)
    {
        RNSData data;
        data.name = rnsNames[i];
        data.hexID = rnsHexID[i];
        data.routingID = rnsRoutingID[i];
        data.online = false;
        nodeDataMap[rnsNames[i]] = data;
    }
}

// Function to access node data safely
RNSData getRNSData(const std::string &nodeName)
{
    std::lock_guard<std::mutex> lock(mapMutex);
    if (nodeDataMap.find(nodeName) != nodeDataMap.end())
    {
        return nodeDataMap[nodeName];
    }
    else
    {
        return {"", "", false, ""}; // Return empty structure if node not found
    }
}

// Get RNS Online Node Data
std::string getRNSHexID(const std::string &nodeName)
{
    std::lock_guard<std::mutex> lock(mapMutex);
    auto it = nodeDataMap.find(nodeName);
    if (it != nodeDataMap.end() && it->second.online)
    {
        return it->second.hexID;
    }
    else
    {
        return ""; // Return an empty string if not found or offline
    }
}

// Function to print all node statuses
void printNodeStatuses(quill::Logger *logger)
{
    std::lock_guard<std::mutex> lock(mapMutex);
    for (const auto &pair : nodeDataMap)
    {
        const RNSData &data = pair.second;
        LOG_INFO(logger, "Node: {} - HexID: {} - RoutingID: {} - Online: {}", data.name, data.hexID, data.routingID, data.online);
    }
}