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
#include "serial_data.h"
#include "thread_safe_queue.hpp"

void send_data(const std::string &destination_hexhash, const std::string &configpath, const std::string &text);
void rns_sender(ThreadSafeQueue<std::pair<std::string, FlatBufferData>> &dataQueueSend);

#endif // RNS_SENDER_H
