#ifndef RNS_SENDER_H
#define RNS_SENDER_H

#include <cstdlib> // For setenv
#include <iostream>
#include <thread>
#include <pybind11/embed.h>
#include "test_generated.h"
#include <vector>
#include <string>
#include "thread_safe_queue.hpp"

void send_data(const std::string &destination_hexhash, const std::string &configpath, const std::string &text);
void rns_sender_manager_threaded(ThreadSafeQueue<std::tuple<std::string, std::string, std::string>> &dataQueue);

#endif // RNS_SENDER_H
