#include "rns_sender.h"

#include <cstdlib> // For setenv
#include <iostream>
#include <thread>
#include <pybind11/embed.h>
#include "test_generated.h"
#include <vector>
#include <string>

namespace py = pybind11;

void send_data(const std::string &destination_hexhash, const std::string &configpath, const std::string &text)
{
    try
    {
        py::module_ sender = py::module_::import("rns_sender");
        auto client = sender.attr("client");

        flatbuffers::FlatBufferBuilder builder;
        auto text_offset = builder.CreateString(text);
        auto message = Example::CreateMessage(builder, text_offset);
        builder.Finish(message);

        // Convert FlatBuffer to Python bytes and call Python function
        client(destination_hexhash, configpath, py::bytes(reinterpret_cast<const char *>(builder.GetBufferPointer()), builder.GetSize()));
    }
    catch (const py::error_already_set &e)
    {
        std::cerr << "Python error: " << e.what() << std::endl;
        throw;
    }
}

void rns_sender_manager(ThreadSafeQueueNetwork<std::tuple<std::string, std::string, std::string>> &dataQueue)
{
    py::gil_scoped_acquire acquire; // Acquire the GIL since we are using Python
    while (true)
    {
        auto [destination_hexhash, configpath, text] = dataQueue.pop();
        send_data(destination_hexhash, configpath, text);
    }
}

// void send_data(const std::string &destination_hexhash, const std::string &configpath, const std::string &text)
// {
//     try
//     {
//         py::module_ sender = py::module_::import("test");
//         auto client = sender.attr("client");

//         flatbuffers::FlatBufferBuilder builder;
//         auto text_offset = builder.CreateString(text);
//         auto message = Example::CreateMessage(builder, text_offset);
//         builder.Finish(message);

//         // Convert FlatBuffer to Python bytes and call Python function
//         client(destination_hexhash, configpath, py::bytes(reinterpret_cast<const char *>(builder.GetBufferPointer()), builder.GetSize()));
//     }
//     catch (const py::error_already_set &e)
//     {
//         std::cerr << "Python error: " << e.what() << std::endl;
//         throw;
//     }
// }

// void rns_sender_manager()
// {

//     py::gil_scoped_acquire acquire;

//     send_data("d8301c848ab69378f7215e729e4325cf", "/Users/rpaillet/.reticulum", "Hello, Reticulum!");
// }