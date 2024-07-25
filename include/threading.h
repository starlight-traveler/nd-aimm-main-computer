#ifndef THREADING_H
#define THREADING_H

#include "logger.h"
#include <functional>
#include <chrono>
#include <thread>

// Define a template function to run any function with retries
template <typename Func, typename... Args>
void threaded(quill::Logger *logger, int retry_delay_sec, Func func, Args &&...args);

#include "threading.tpp" // Include the template implementation

#endif // THREADING_H
