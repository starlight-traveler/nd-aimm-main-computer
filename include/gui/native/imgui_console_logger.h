#include "logger.h"
#include <mutex>
#include <deque>
#include <string>

// Example ImGui console interface
class ImGuiConsole
{
public:
    void AddLog(const char *fmt, ...) IM_FMTARGS(2)
    {
        // Store messages in a deque or similar structure
        va_list args;
        va_start(args, fmt);
        char buf[1024];
        vsnprintf(buf, sizeof(buf), fmt, args);
        buf[sizeof(buf) - 1] = '\0';
        va_end(args);
        _logs.emplace_back(buf);
    }

    std::deque<std::string> _logs;
};

// Custom sink for ImGui
class ImGuiSink : public quill::Handler
{
public:
    explicit ImGuiSink(ImGuiConsole *console) : _console(console) {}

    void emit(quill::LogRecord const &log_record, quill::fmt::memory_buffer const &formatted_log_record) override
    {
        // Assuming _console is thread-safe or only accessed from the GUI thread
        _console->AddLog("%s", formatted_log_record.data());
    }

    void flush() override {}

private:
    ImGuiConsole *_console;
};
