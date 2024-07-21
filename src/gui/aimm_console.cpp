
#include "runner.h"
#include "console.h"
#include "logger_monitor.h"

#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/Logger.h"
#include "quill/sinks/ConsoleSink.h"
#include "quill/sinks/FileSink.h"

struct AIMMConsole
{
    char InputBuf[256];
    ImVector<char *> Items;
    ImVector<const char *> Commands;
    ImVector<char *> History;
    int HistoryPos; // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter Filter;
    bool AutoScroll;
    bool ScrollToBottom;
    bool quillOutput = true;

    AIMMConsole()
    {
        ClearLog();
        memset(InputBuf, 0, sizeof(InputBuf));
        HistoryPos = -1;

        Commands.push_back("help");
        Commands.push_back("history");
        Commands.push_back("clear");
        Commands.push_back("log");
        AutoScroll = true;
        ScrollToBottom = false;
    }
    ~AIMMConsole()
    {
        ClearLog();
        for (int i = 0; i < History.Size; i++)
            ImGui::MemFree(History[i]);
    }

    // Portable helpers
    static int Stricmp(const char *s1, const char *s2)
    {
        int d;
        while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
        {
            s1++;
            s2++;
        }
        return d;
    }
    static int Strnicmp(const char *s1, const char *s2, int n)
    {
        int d = 0;
        while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
        {
            s1++;
            s2++;
            n--;
        }
        return d;
    }
    static char *Strdup(const char *s)
    {
        IM_ASSERT(s);
        size_t len = strlen(s) + 1;
        void *buf = ImGui::MemAlloc(len);
        IM_ASSERT(buf);
        return (char *)memcpy(buf, (const void *)s, len);
    }
    static void Strtrim(char *s)
    {
        char *str_end = s + strlen(s);
        while (str_end > s && str_end[-1] == ' ')
            str_end--;
        *str_end = 0;
    }

    void ClearLog()
    {
        for (int i = 0; i < Items.Size; i++)
            ImGui::MemFree(Items[i]);
        Items.clear();
    }

    void AddLog(const char *fmt, ...) IM_FMTARGS(2)
    {
        // FIXME-OPT
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);
        Items.push_back(Strdup(buf));
    }

    void Draw(const char *title, bool *p_open)
    {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
        // So e.g. IsItemHovered() will return true when hovering the title bar.
        // Here we create a context menu only available from the title bar.
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Close Console"))
                *p_open = false;
            ImGui::EndPopup();
        }

        ImGui::TextWrapped(
            "This is a limited emulation of a console based interface that allows for manual commands to be sent to the main computer. Use sparingly as "
            "the main system will read these commands in a threaded context and execute them. It may decided to override the command so if you go into "
            "override mode in the console to supersede the system determination.");
        ImGui::TextWrapped("Enter 'HELP' for help.");

        // TODO: Add logging

        try
        {
            auto logMonitor = LogFileMonitor::GetInstance("/Users/rpaillet/Documents/Projects/NDAIMM/nd-aimm/nd-aimm-main-computer/build/aimm_logging.log");

            logMonitor->UpdateLogBuffer();

            std::string logEntry;
            bool foundLogs = false;

            while (!(logEntry = logMonitor->GetNextLogEntry()).empty() && quillOutput)
            {
                AddLog(logEntry.c_str()); // Process each log entry with your external AddLog function
                foundLogs = true;
            }

        }
        catch (const std::runtime_error &e)
        {
            std::cerr << "Runtime error: " << e.what() << std::endl;
        }

        if (ImGui::SmallButton("Help"))
        {
            AddLog("==========================================================================");
            AddLog("Commands List:");
            AddLog("- help: Displays this help message with information on all available commands.");
            AddLog("- status: Shows the current status of the application, including active connections and system health.");
            AddLog( "- restart: Safely restarts the application, reinitializing all modules without disrupting ongoing tasks.");
            AddLog("- exit: Closes the application cleanly, ensuring all data is saved and resources are released properly.");
            AddLog("- log <level>: Sets the logging level of the system. Available levels are DEBUG, INFO, WARN, ERROR.");
            AddLog("- update: Checks for updates to the application and applies them if available.");
            AddLog("For more details on each command, type 'help <command>'. For example, 'help status' will provide detailed information about the 'status' command.");
            AddLog("==========================================================================");
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("Clear"))
        {
            ClearLog();
        }
        ImGui::SameLine();
        bool copy_to_clipboard = ImGui::SmallButton("Copy");
        // static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

        ImGui::Separator();

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        // Options, Filter
        ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_O, ImGuiInputFlags_Tooltip);
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        if (ImGui::Checkbox("Logger", &quillOutput))
        {
        }
        ImGui::SameLine();
        Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        ImGui::Separator();

        // Reserve enough left-over height for 1 separator + 1 input text
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::Selectable("Clear"))
                    ClearLog();
                ImGui::EndPopup();
            }

            // Display every line as a separate entry so we can change their color or add custom widgets.
            // If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
            // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
            // to only process visible items. The clipper will automatically measure the height of your first item and then
            // "seek" to display only items in the visible area.
            // To use the clipper we can replace your standard loop:
            //      for (int i = 0; i < Items.Size; i++)
            //   With:
            //      ImGuiListClipper clipper;
            //      clipper.Begin(Items.Size);
            //      while (clipper.Step())
            //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            // - That your items are evenly spaced (same height)
            // - That you have cheap random access to your elements (you can access them given their index,
            //   without processing all the ones before)
            // You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
            // We would need random-access on the post-filtered list.
            // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
            // or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
            // and appending newly elements as they are inserted. This is left as a task to the user until we can manage
            // to improve this example code!
            // If your items are of variable height:
            // - Split them into same height items would be simpler and facilitate random-seeking into your list.
            // - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
            if (copy_to_clipboard)
                ImGui::LogToClipboard();
            for (const char *item : Items)
            {
                if (!Filter.PassFilter(item))
                    continue;

                // Normally you would store more information in your item than just a string.
                // (e.g. make Items[] an array of structure, store color/type etc.)
                ImVec4 color;
                bool has_color = false;
                // Check for different log levels and assign colors
                if (strstr(item, "LOG_ERROR") || strstr(item, "[error]")) // Error messages in red
                {
                    color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); // Red
                    has_color = true;
                }
                else if (strstr(item, "LOG_WARN")) // Warning messages in yellow
                {
                    color = ImVec4(1.0f, 1.0f, 0.4f, 1.0f); // Yellow
                    has_color = true;
                }
                else if (strstr(item, "LOG_INFO")) // Info messages in blue
                {
                    color = ImVec4(0.4f, 0.4f, 1.0f, 1.0f); // Blue
                    has_color = true;
                }
                else if (strstr(item, "LOG_DEBUG")) // Debug messages in cyan
                {
                    color = ImVec4(0.4f, 1.0f, 1.0f, 1.0f); // Cyan
                    has_color = true;
                }
                else if (strstr(item, "LOG_TRACE_L1") || strstr(item, "LOG_TRACE_L2") || strstr(item, "LOG_TRACE_L3")) // Trace messages in green
                {
                    color = ImVec4(0.4f, 1.0f, 0.4f, 1.0f); // Green
                    has_color = true;
                }
                else if (strncmp(item, "# ", 2) == 0) // Special case handled from the previous code
                {
                    color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); // Light orange
                    has_color = true;
                }
                if (has_color)
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(item);
                if (has_color)
                    ImGui::PopStyleColor();
            }
            if (copy_to_clipboard)
                ImGui::LogFinish();

            // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
            // Using a scrollbar or mouse-wheel will take away from the bottom edge.
            if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                ImGui::SetScrollHereY(1.0f);
            ScrollToBottom = false;

            ImGui::PopStyleVar();
        }
        ImGui::EndChild();
        ImGui::Separator();

        // Command-line
        bool reclaim_focus = false;
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
        if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub, (void *)this))
        {
            char *s = InputBuf;
            Strtrim(s);
            if (s[0])
                ExecCommand(s);
            strcpy(s, "");
            reclaim_focus = true;
        }

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        ImGui::End();
    }

    void ExecCommand(const char *command_line)
    {
        AddLog("# %s\n", command_line);

        // Frontend
        quill::ConsoleColours custom_console_colours;
        custom_console_colours.set_default_colours();
        custom_console_colours.set_colour(quill::LogLevel::Info, quill::ConsoleColours::blue);      // overwrite the colour for INFO
        custom_console_colours.set_colour(quill::LogLevel::Warning, quill::ConsoleColours::yellow); // overwrite the colour for WARNING
        custom_console_colours.set_colour(quill::LogLevel::Error, quill::ConsoleColours::red);      // overwrite the colour for ERROR

        auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("sink_id_1", custom_console_colours);

        auto file_sink = quill::Frontend::create_or_get_sink<quill::FileSink>(
            "aimm_logging.log",
            []()
            {
                quill::FileSinkConfig cfg;
                cfg.set_open_mode('w');
                return cfg;
            }(),
            quill::FileEventNotifier{});

        quill::Logger *logger = quill::Frontend::create_or_get_logger("root", {std::move(console_sink), std::move(file_sink)});

        // Insert into history. First find match and delete it so it can be pushed to the back.
        // This isn't trying to be smart or optimal.
        HistoryPos = -1;
        for (int i = History.Size - 1; i >= 0; i--)
            if (Stricmp(History[i], command_line) == 0)
            {
                ImGui::MemFree(History[i]);
                History.erase(History.begin() + i);
                break;
            }
        History.push_back(Strdup(command_line));

        // Process command
        if (Stricmp(command_line, "clear") == 0)
        {
            ClearLog();
        }
        else if (Stricmp(command_line, "help") == 0)
        {
            AddLog("==========================================================================");
            AddLog("Commands List:");
            AddLog("- help: Displays this help message with information on all available commands.");
            AddLog("- history: Displays history of commands inputted.");
            AddLog("- clear: Clears the display.");
            AddLog("- log <level>: Sets the logging level of the system. Available levels are DEBUG, INFO, WARN, ERROR.");
            AddLog("For more details on each command, type 'help <command>'. For example, 'help status' will provide detailed information about the 'status' command.");
            AddLog("==========================================================================");
        }
        else if (Stricmp(command_line, "history") == 0)
        {
            int first = History.Size - 10;
            for (int i = first > 0 ? first : 0; i < History.Size; i++)
                AddLog("%3d: %s\n", i, History[i]);
        }
        else if (Stricmp(command_line, "log trace_l3") == 0)
        {
            logger->set_log_level(quill::LogLevel::TraceL3);
        }
        else if (Stricmp(command_line, "log trace_l2") == 0)
        {
            logger->set_log_level(quill::LogLevel::TraceL2);
        }
        else if (Stricmp(command_line, "log trace_l1") == 0)
        {
            logger->set_log_level(quill::LogLevel::TraceL1);
        }
        else if (Stricmp(command_line, "log debug") == 0)
        {
            logger->set_log_level(quill::LogLevel::Debug);
        }
        else if (Stricmp(command_line, "log warn") == 0)
        {
            logger->set_log_level(quill::LogLevel::Warning);
        }
        else if (Stricmp(command_line, "log info") == 0)
        {
            logger->set_log_level(quill::LogLevel::Info);
            
        }
        else if (Stricmp(command_line, "log error") == 0)
        {
            logger->set_log_level(quill::LogLevel::Error);
        }
        else if (Stricmp(command_line, "log critical") == 0)
        {
            logger->set_log_level(quill::LogLevel::Critical);
        }
        else
        {
            AddLog("Unknown command: '%s'\n", command_line);
        }

        // On command input, we scroll to bottom even if AutoScroll==false
        ScrollToBottom = true;
    }

    // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
    static int TextEditCallbackStub(ImGuiInputTextCallbackData *data)
    {
        AIMMConsole *console = (AIMMConsole *)data->UserData;
        return console->TextEditCallback(data);
    }

    int TextEditCallback(ImGuiInputTextCallbackData *data)
    {
        AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
        switch (data->EventFlag)
        {
        case ImGuiInputTextFlags_CallbackCompletion:
        {
            // Example of TEXT COMPLETION

            // Locate beginning of current word
            const char *word_end = data->Buf + data->CursorPos;
            const char *word_start = word_end;
            while (word_start > data->Buf)
            {
                const char c = word_start[-1];
                if (c == ' ' || c == '\t' || c == ',' || c == ';')
                    break;
                word_start--;
            }

            // Build a list of candidates
            ImVector<const char *> candidates;
            for (int i = 0; i < Commands.Size; i++)
                if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
                    candidates.push_back(Commands[i]);

            if (candidates.Size == 0)
            {
                // No match
                AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
            }
            else if (candidates.Size == 1)
            {
                // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                data->InsertChars(data->CursorPos, candidates[0]);
                data->InsertChars(data->CursorPos, " ");
            }
            else
            {
                // Multiple matches. Complete as much as we can..
                // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                int match_len = (int)(word_end - word_start);
                for (;;)
                {
                    int c = 0;
                    bool all_candidates_matches = true;
                    for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                        if (i == 0)
                            c = toupper(candidates[i][match_len]);
                        else if (c == 0 || c != toupper(candidates[i][match_len]))
                            all_candidates_matches = false;
                    if (!all_candidates_matches)
                        break;
                    match_len++;
                }

                if (match_len > 0)
                {
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                }

                // List matches
                AddLog("Possible matches:\n");
                for (int i = 0; i < candidates.Size; i++)
                    AddLog("- %s\n", candidates[i]);
            }

            break;
        }

        case ImGuiInputTextFlags_CallbackHistory:
        {
            // Example of HISTORY
            const int prev_history_pos = HistoryPos;
            if (data->EventKey == ImGuiKey_UpArrow)
            {
                if (HistoryPos == -1)
                    HistoryPos = History.Size - 1;
                else if (HistoryPos > 0)
                    HistoryPos--;
            }
            else if (data->EventKey == ImGuiKey_DownArrow)
            {
                if (HistoryPos != -1)
                    if (++HistoryPos >= History.Size)
                        HistoryPos = -1;
            }

            // A better implementation would preserve the data on the current input line along with cursor position.
            if (prev_history_pos != HistoryPos)
            {
                const char *history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, history_str);
            }
        }
        }
        return 0;
    }
};

void showAIMMConsole(bool *p_open)
{
    static AIMMConsole console;
    console.Draw("AIMM Console", p_open);
}