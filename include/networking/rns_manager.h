#pragma once

#include "subprocess.hpp"
#include "logger.h"
#include "threading.tpp"
#include <pybind11/embed.h>
#include "serial_data.h"
#include "thread_safe_queue.hpp"

namespace py = pybind11;

// Define a structure for node data
struct RNSData
{
    std::string name;
    std::string hex_id;
    bool online;
    std::string routing_id;
};

// Function prototypes
void rnsd_daemon(quill::Logger *logger);
void rns_receiver_manager(quill::Logger *logger);
void rns_sender_manager(ThreadSafeQueue<std::pair<std::string, FlatBufferData>> &dataQueueSend, quill::Logger *logger);
void verify_connection(const std::string &hexadecimal, quill::Logger *logger);
void initialize_nodes();
void print_node_statuses(quill::Logger *logger);
RNSData get_rns_data(const std::string &node_name);
std::string get_rns_hex_id(const std::string &node_name);

extern const std::vector<std::string> rns_names;
extern const std::vector<std::string> rns_hex_ids;
extern const std::vector<std::string> rns_routing_ids;
extern std::map<std::string, RNSData> node_data_map;
extern std::mutex map_mutex;