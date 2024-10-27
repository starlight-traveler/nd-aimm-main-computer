#include "rns_sender.h"

#include <cstdlib> // For setenv
#include <iostream>
#include <thread>
#include <pybind11/embed.h>
#include <vector>
#include <string>
#include "serial_data.h"
#include "rns_mapping.h"

namespace py = pybind11;

void send_data(const std::string &destination_hexhash, const std::string &configpath, const uint8_t *buffer, size_t size)
{
    try
    {
        py::module_ sender = py::module_::import("rns_sender");
        auto client = sender.attr("client");

        // Pass the FlatBuffer to the Python client
        client(destination_hexhash, configpath, py::bytes(reinterpret_cast<const char *>(buffer), size));
    }
    catch (const py::error_already_set &e)
    {
        std::cerr << "Python error: " << e.what() << std::endl;
        throw;
    }
}

void rns_sender_manager_threaded(ThreadSafeQueue<std::pair<std::string, FlatBufferData>> &dataQueueSend)
{
    py::gil_scoped_acquire acquire; // Acquire the GIL since we are using Python
    while (true)
    {
        auto [identifier, buffer] = dataQueueSend.pop();
        auto [configpath, destination_hexhash] = get_config_and_hash(identifier);
        send_data(destination_hexhash, configpath, buffer.data(), buffer.size());
    }
}

// void rns_sender_manager()
// {

//     py::gil_scoped_acquire acquire;

//     send_data("d8301c848ab69378f7215e729e4325cf", "/Users/rpaillet/.reticulum", "Hello, Reticulum!");
// }