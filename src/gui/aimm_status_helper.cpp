#include "runner.h"
#include "aimm_test_manager.h"

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
        ImGui::Text("%s", buf);
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
    ImGui::SetColumnWidth(2, 160);            // Allow more space for varied status messages

    // Adding headers for each column
    ImGui::Text("Status");
    ImGui::NextColumn();
    ImGui::Text("Description");
    ImGui::NextColumn();
    ImGui::Text("Result");
    ImGui::NextColumn();
}

void AIMMBasic::RenderTestRow(std::string category)
{

    auto tests = TestManager::getInstance()->getTests(category);

    for (const auto &test : tests)
    {
        // Draw the button centered in the first column
        float columnWidth = ImGui::GetColumnWidth(-1);
        float buttonSize = 20.0f;
        float buttonPosX = (columnWidth - buttonSize) * 0.5f;
        float cursorPosY = ImGui::GetCursorPosY();

        ImGui::SetCursorPosX(buttonPosX);
        ImGui::SetCursorPosY(cursorPosY);

        ImVec4 color = test->getColor();
        bool isRunning = test->isRunning();
        std::string id = "##" + test->getDescription(); // Unique ID for each button

        // Assuming `true` for running, replace with actual condition
        TestStatusButton(id.c_str(), color, isRunning, -1);
        ImGui::SetCursorPosY(cursorPosY + buttonSize);
        ImGui::NextColumn();

        ImGui::TextWrapped("%s", test->getDescription().c_str());
        ImGui::NextColumn();

        ImGui::TextColored(color, "%s", test->getResult().c_str());
        ImGui::NextColumn();
    }

    ImGui::Columns(1); // Reset to one column layout after use
}