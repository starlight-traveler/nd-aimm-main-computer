#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include <pybind11/embed.h>
#include "subprocess.hpp"
#include "logger.h"
#include "quill/LogMacros.h"
#include "threading.tpp"

using subprocess::CompletedProcess;
using subprocess::PipeOption;
using subprocess::Popen;
using subprocess::RunBuilder;

namespace py = pybind11;

// FIXME: RNS Node system does not take into account hash tables

/**
 * @brief RNS Start Manager
 * 
 */

void rnsd_dameon(quill::Logger *logger)
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
        buf[bytes_read] = '\0';           // Null-terminate the string
        LOG_INFO(logger, "RNSD: {}", buf); 
        std::memset(buf, 0, sizeof(buf)); // Clear the buffer
    }

    // // Close the process and handle cleanup
    // popen.close();
}

/**
 * @brief RNS Reciever Manager
 * 
 */




/**
 * @brief RNS Sender Manager
 * 
 */

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

void verify_connection(const std::string &hexadecimal, quill::Logger *logger)
{
    try
    {
        py::module_ verifier = py::module_::import("rns_verifier");
        auto rnpath = verifier.attr("run_rnpath");

        // Convert FlatBuffer to Python bytes and call Python function
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
    // The external node information hashes, these are public keys so it is fine
    // to include them

    // If anyone wants to take on the challenge to change these to be added dynamically
    // at runtime please do, this is technically in violation of the RNS Network Stack
    // rules

    // Do not change or add to unless told
    std::vector<std::string> rnsNames = {"nd-aimm"};
    std::vector<std::string> rnsHexID = {"cb02f08466a9cac03d6523c504ac1879"};
    std::vector<std::string> rnsRoutingID = {"10858d74bc5d400beb73d691181ca1e9"};

    if (rnsNames.size() != rnsHexID.size() || rnsNames.size() != rnsRoutingID.size())
    {
        std::cerr << "Error: Data vectors must be of the same length." << std::endl;
        return; // Early return to avoid processing mismatched data.
    }

    std::lock_guard<std::mutex> lock(mapMutex);
    for (size_t i = 0; i < rnsNames.size(); ++i)
    {
        RNSData data;
        data.name = rnsNames[i];
        data.hexID = rnsHexID[i];         // Assign corresponding hex ID
        data.routingID = rnsRoutingID[i]; // Assign corresponding routing ID
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
        return {"", "", false}; // Return empty structure if node not found
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

// Example usage: function to print all node statuses
void printNodeStatuses(quill::Logger *logger)
{
    std::lock_guard<std::mutex> lock(mapMutex);
    for (const auto &pair : nodeDataMap)
    {
        const RNSData &data = pair.second;

        LOG_INFO(logger, "Node: {} - HexID: {} - RoutingID: {} - Online: {}", data.name, data.hexID, data.routingID, data.online);
    }
}


void rns_sender_manager(quill::Logger *logger)
{
    py::gil_scoped_acquire acquire;

    initializeNodes();   // Initialize the node data
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
                node.online = true;            // Update the node's online status if verification is successful
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