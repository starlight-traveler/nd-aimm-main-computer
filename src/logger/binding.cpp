#include <pybind11/pybind11.h>
#include <utility>

#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/Logger.h"
#include "quill/sinks/ConsoleSink.h"
#include "quill/sinks/FileSink.h"
#include "quill/LogMacros.h"
#include "quill/Logger.h"

// Enum class for log levels
enum class LogLevel
{
    Info,
    Warning,
    Error
};

// Global initialization for Quill backend and logger
quill::Logger *global_logger = nullptr;

void initialize_logger_python()
{

    // Start the backend thread
    quill::BackendOptions backend_options;
    quill::Backend::start(backend_options);

    // Create custom console colors
    quill::ConsoleColours console_colours;
    console_colours.set_default_colours();
    console_colours.set_colour(quill::LogLevel::Info, quill::ConsoleColours::blue);
    console_colours.set_colour(quill::LogLevel::Warning, quill::ConsoleColours::yellow);
    console_colours.set_colour(quill::LogLevel::Error, quill::ConsoleColours::red);

    // Create the sink
    auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("sink_id2", console_colours);

    // Console, logger
    global_logger = quill::Frontend::create_or_get_logger("python3", {std::move(console_sink)});

    // Set the log level to print everything
    global_logger->set_log_level(quill::LogLevel::TraceL3);
}

// Function to log a message at a specified log level
void log_message(const std::string &message, LogLevel level)
{
    if (!global_logger)
    {
        throw std::runtime_error("Global logger not initialized.");
    }

    // Log the message at the appropriate level
    switch (level)
    {
    case LogLevel::Info:
        LOG_INFO(global_logger, "{}", message);
        break;
    case LogLevel::Warning:
        LOG_WARNING(global_logger, "{}", message);
        break;
    case LogLevel::Error:
        LOG_ERROR(global_logger, "{}", message);
        break;
    }
}

// Pybind11 module definition
PYBIND11_MODULE(quill_logger, m)
{

    pybind11::gil_scoped_acquire acquire;

    // Initialize the logger only once
    initialize_logger_python();


    pybind11::enum_<LogLevel>(m, "LogLevel")
        .value("Info", LogLevel::Info)
        .value("Warning", LogLevel::Warning)
        .value("Error", LogLevel::Error)
        .export_values();

    m.def("log_message", &log_message, "Logs a message using Quill", pybind11::arg("message"), pybind11::arg("level"));
}
