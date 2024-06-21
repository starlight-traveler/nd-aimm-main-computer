#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/Logger.h"
#include "quill/sinks/ConsoleSink.h"

#include <utility>

quill::Logger *initialize_logger()
{
    // Start the backend thread
    quill::BackendOptions backend_options;
    quill::Backend::start(backend_options);

    // Frontend
    quill::ConsoleColours custom_console_colours;
    custom_console_colours.set_default_colours();
    custom_console_colours.set_colour(quill::LogLevel::Info, quill::ConsoleColours::blue);      // overwrite the colour for INFO
    custom_console_colours.set_colour(quill::LogLevel::Warning, quill::ConsoleColours::yellow); // overwrite the colour for WARNING
    custom_console_colours.set_colour(quill::LogLevel::Error, quill::ConsoleColours::red);      // overwrite the colour for ERROR

    // Create the sink
    auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("sink_id_1", custom_console_colours);

    quill::Logger *logger = quill::Frontend::create_or_get_logger("root", std::move(console_sink));

    // Change the LogLevel to print everything
    logger->set_log_level(quill::LogLevel::TraceL3);

    return logger;
}