#include "runner.h"

void AIMMBasic::DrawStatusWindow(bool *s_open)
{
    if (!ImGui::Begin("Status", s_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    if (ImGui::BeginMenuBar()) // Begin menu bar block
    {
        if (ImGui::BeginMenu("General")) // Menu dropdown
        {
            ImGui::MenuItem("Overview");
            ImGui::MenuItem("Power");
            ImGui::MenuItem("All");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Microelectronics"))
        {
            ImGui::MenuItem("Peripherals");
            ImGui::MenuItem("Radio Frequency");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Computational"))
        {
            ImGui::MenuItem("Startup");
            ImGui::MenuItem("Runtime");
            ImGui::MenuItem("Flatbuffers");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Mechanics"))
        {
            ImGui::MenuItem("Location");
            ImGui::MenuItem("Water Detection");

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Cycle colors based on time
    auto currentTime = std::chrono::steady_clock::now().time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
    colorIndex = (seconds / 2) % colors.size(); // Change color every 2 seconds

    // Example status counts, these would typically come from your application logic
    int numPasses = 20;
    int numWarnings = 5;
    int numErrors = 3;
    std::string operationStatus = "Operational";

    AIMMBasic::TestStatusColumnsAndOverallHealth(numPasses, numWarnings, numErrors, operationStatus);

    ImGui::Separator(); // Add a separator after the headers for clarity

    // Draw the button centered in the first column
    float columnWidth = ImGui::GetColumnWidth(-1); // Get the width of the current column
    float buttonSize = 20.0f;                      // Assuming the button is a 40x40 square

    // Calculate the center position for the button
    float buttonPosX = (columnWidth - buttonSize) * 0.5f;
    float cursorPosY = ImGui::GetCursorPosY(); // Current vertical cursor position

    ImGui::SetCursorPosX(buttonPosX);
    ImGui::SetCursorPosY(cursorPosY);

    // Improved formatting for the content rows
    TestStatusButton("colorButton1", colors[colorIndex], true, -1);
    ImGui::SetCursorPosY(cursorPosY + buttonSize); // Move cursor below the button
    ImGui::NextColumn();

    ImGui::TextWrapped("This is a test description that can be longer and more detailed, explaining the status and purpose of the test currently being displayed."); // More detailed description
    ImGui::NextColumn();

    ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "PASS"); // Green text for positive status
    ImGui::NextColumn();

    ImGui::Columns(1); // Reset columns to single column layout after use

    ImGui::Columns(1); // Reset to one column

    ImGui::End();
}

void AIMMBasic::TestStatusButton(const char *id, const ImVec4 &color, bool running, int display_counter)
{
    ImVec2 size = ImVec2(20.0f, 20.0f);
    ImGui::PushItemFlag(ImGuiItemFlags_NoTabStop, true);
    ImGui::ColorButton(id, color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, size);
    ImGui::PopItemFlag();

    if (running)
    {
        ImVec2 center = ImGui::GetItemRectMin();
        center.x += ImGui::GetItemRectSize().x / 2;
        center.y += ImGui::GetItemRectSize().y / 2;
        float radius = ImGui::GetItemRectSize().x / 2 * 0.5f; // Smaller radius
        float t = (float)(ImGui::GetTime() * 2.0f);
        ImVec2 off = ImVec2(cos(t) * radius, sin(t) * radius);
        ImVec2 line_start = ImVec2(center.x - off.x, center.y - off.y);
        ImVec2 line_end = ImVec2(center.x + off.x, center.y + off.y);
        ImGui::GetWindowDrawList()->AddLine(line_start, line_end, ImGui::GetColorU32(ImGuiCol_Text), 2.0f);
    }
    else if (display_counter >= 0)
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d", display_counter);
        ImGui::SameLine();
        ImGui::Text(buf);
    }
}

void AIMMBasic::TestStatusColumnsAndOverallHealth(int numPasses, int numWarnings, int numErrors, std::string operationalStatus)
{


    ImGui::Text("System Status: ");
    ImGui::SameLine();
    // Display the check icon in green
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", ICON_FA_CIRCLE_CHECK); // Green for "Passed"
    ImGui::SameLine();
    ImGui::Text("%d", numPasses);
    ImGui::SameLine();

    // Display the warning icon in yellow
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", ICON_FA_TRIANGLE_EXCLAMATION); // Yellow for "Warning"
    ImGui::SameLine();
    ImGui::Text("%d", numWarnings);
    ImGui::SameLine();

    // Display the error icon in red
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", ICON_FA_CIRCLE_EXCLAMATION); // Red for "Error"
    ImGui::SameLine();
    ImGui::Text("%d", numErrors);
    ImGui::SameLine();

    ImGui::Text(" -- Overall Health:");
    ImGui::SameLine();
    ImGui::Text("%s", operationalStatus.c_str());

    ImGui::Separator(); // Draws a line to separate the summary from the columns

    ImGui::Columns(3, "statusColumns", true); // Enable border for clearer separation
    ImGui::SetColumnWidth(0, 60);             // Adjusted width for better visual balance
    ImGui::SetColumnWidth(1, 320);            // Slightly larger to accommodate longer descriptions
    ImGui::SetColumnWidth(2, 120);            // Allow more space for varied status messages

    // Adding headers for each column
    ImGui::Text("Status");
    ImGui::NextColumn();
    ImGui::Text("Description");
    ImGui::NextColumn();
    ImGui::Text("Result");
    ImGui::NextColumn();
}