#include <pybind11/pybind11.h>
#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/Logger.h"
#include "quill/sinks/ConsoleSink.h"
#include "quill/LogMacros.h"

// Enum class for log levels
enum class LogLevel
{
    Info,
    Warning,
    Error
};

void log_message(const std::string &message, LogLevel level)
{
    // Start the backend thread
    quill::BackendOptions backend_options;
    quill::Backend::start(backend_options);

    // Frontend setup with custom colors
    quill::ConsoleColours custom_console_colours;
    custom_console_colours.set_default_colours();
    custom_console_colours.set_colour(quill::LogLevel::Info, quill::ConsoleColours::blue);
    custom_console_colours.set_colour(quill::LogLevel::Warning, quill::ConsoleColours::yellow);
    custom_console_colours.set_colour(quill::LogLevel::Error, quill::ConsoleColours::red);

    // Create the sink
    auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("sink_id_2", custom_console_colours);

    // Create or get the logger
    quill::Logger *logger = quill::Frontend::create_or_get_logger("py_logger", console_sink);
    if (!logger)
    {
        throw std::runtime_error("Failed to create or get logger.");
    }

    // Set the log level to print everything
    logger->set_log_level(quill::LogLevel::TraceL3);

    // Log the message at the appropriate level
    switch (level)
    {
    case LogLevel::Info:
        LOG_INFO(logger, "{}", message);
        break;
    case LogLevel::Warning:
        LOG_WARNING(logger, "{}", message);
        break;
    case LogLevel::Error:
        LOG_ERROR(logger, "{}", message);
        break;
    }
}

PYBIND11_MODULE(quill_logger, m)
{
    pybind11::enum_<LogLevel>(m, "LogLevel")
        .value("Info", LogLevel::Info)
        .value("Warning", LogLevel::Warning)
        .value("Error", LogLevel::Error)
        .export_values();

    m.def("log_message", &log_message, "Logs a message using Quill", pybind11::arg("message"), pybind11::arg("level"));
}
