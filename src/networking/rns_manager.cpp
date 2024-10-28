#include "rns_manager.h"
#include "rns_sender.h"

// Using declarations
using subprocess::CompletedProcess;
using subprocess::PipeOption;
using subprocess::Popen;
using subprocess::RunBuilder;

namespace py = pybind11;

// Declare global variables before use
const std::vector<std::string> rns_names = {"nd-aimm"};
const std::vector<std::string> rns_hex_ids = {"cb02f08466a9cac03d6523c504ac1879"};
const std::vector<std::string> rns_routing_ids = {"10858d74bc5d400beb73d691181ca1e9"};

// Global map to hold node data, and a mutex to protect it
std::map<std::string, RNSData> node_data_map;
std::mutex map_mutex;

/**
 * @brief Daemon
 */
void rnsd_daemon(quill::Logger *logger)
{
    // Start the rnsd subprocess with output piped
    Popen popen = RunBuilder({"rnsd"})
                      .cout(PipeOption::pipe)
                      .popen();

    // Close the process and handle cleanup
    // popen.close();
}

/**
 * @brief Receiver
 */
void rns_receiver_manager(quill::Logger *logger)
{
    // TODO: Implement receiver logic
}

/**
 * @brief Sender
 */
void rns_sender_manager(ThreadSafeQueue<std::pair<std::string, FlatBufferData>> &dataQueueSend, quill::Logger *logger)
{
    py::gil_scoped_acquire acquire;

    initialize_nodes();          // Initialize the node data

    // Loop through all nodes and verify connections
    {
        std::lock_guard<std::mutex> lock(map_mutex); // Lock the mutex once for the entire operation
        for (auto &pair : node_data_map)
        {
            RNSData &node = pair.second;
            try
            {
                verify_connection(node.routing_id, logger); // Verify each node's connection
                node.online = true;                         // Update the node's online status if verification is successful
            }
            catch (const std::exception &e)
            {
                LOG_ERROR(logger, "Failed to verify connection for {} : {}", node.name, e.what());
                node.online = false;
            }
        }
    }


    // Print updated node statuses
    print_node_statuses(logger);

    // Log that we have moved on
    LOG_INFO(logger, "Finished verification of network stack... starting sender.");

    // Sender information
    rns_sender(dataQueueSend);


}

/**
 * @brief Initializer
 */
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
void initialize_nodes()
{
    // Ensure data vectors are of the same length
    if (rns_names.size() != rns_hex_ids.size() || rns_names.size() != rns_routing_ids.size())
    {
        std::cerr << "Error: Data vectors must be of the same length." << std::endl;
        return;
    }

    std::lock_guard<std::mutex> lock(map_mutex);
    for (size_t i = 0; i < rns_names.size(); ++i)
    {
        RNSData data;
        data.name = rns_names[i];
        data.hex_id = rns_hex_ids[i];
        data.routing_id = rns_routing_ids[i];
        data.online = false;
        node_data_map[rns_names[i]] = data;
    }
}

// Function to access node data safely
RNSData get_rns_data(const std::string &node_name)
{
    std::lock_guard<std::mutex> lock(map_mutex);
    if (node_data_map.find(node_name) != node_data_map.end())
    {
        return node_data_map[node_name];
    }
    else
    {
        return {"", "", false, ""}; // Return empty structure if node not found
    }
}

// Get RNS Online Node Data
std::string get_rns_hex_id(const std::string &node_name)
{
    std::lock_guard<std::mutex> lock(map_mutex);
    auto it = node_data_map.find(node_name);
    if (it != node_data_map.end() && it->second.online)
    {
        return it->second.hex_id;
    }
    else
    {
        return ""; // Return an empty string if not found or offline
    }
}

// Function to print all node statuses
void print_node_statuses(quill::Logger *logger)
{
    std::lock_guard<std::mutex> lock(map_mutex);
    for (const auto &pair : node_data_map)
    {
        const RNSData &data = pair.second;
        LOG_INFO(logger, "Node: {} - HexID: {} - RoutingID: {} - Online: {}", data.name, data.hex_id, data.routing_id, data.online);
    }
}
