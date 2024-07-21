#include "runner.h"
#include "thread_safe_gui_status_logger.h"

void AIMMBasic::DrawStatusWindow(bool *s_open)
{
    if (!ImGui::Begin("Status", s_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    if (ImGui::BeginMenuBar()) // Begin menu bar block
    {
        if (ImGui::BeginMenu("General"))
        {
            if (ImGui::MenuItem("Overview"))
            {
                currentMenu = General;
            }

            ImGui::MenuItem("All");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Microelectronics"))
        {
            if (ImGui::MenuItem("Peripherals")) {
                currentMenu = Peripherals;
            }

            if (ImGui::MenuItem("Radio Frequency")) {
                currentMenu = RadioFrequency;
            }

            if (ImGui::MenuItem("Power"))
            {
                currentMenu = Power;
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Computational"))
        {
            if (ImGui::MenuItem("Startup")) {
                currentMenu = Startup;
            }
            
            if (ImGui::MenuItem("Runtime")) {
                currentMenu = Runtime;
            }

            if (ImGui::MenuItem("Flatbuffers")) {
                currentMenu = Flatbuffers;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Mechanics"))
        {

            if (ImGui::MenuItem("Location")) {
                currentMenu = Location;
            }

            if (ImGui::MenuItem("Water Detection")) {
                currentMenu = WaterDetection;
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Display different content based on the selected menu
    switch (currentMenu)
    {
    case General:
        AIMMBasic::DrawGeneralMenu();
        break;
    case Power:
        AIMMBasic::DrawPowerMenu();
        break;
    case Peripherals:
        AIMMBasic::DrawPeripherals();
        break;
    case RadioFrequency:
        AIMMBasic::DrawRadioFrequency();
        break;    
    case Startup:
        AIMMBasic::DrawStartup();
        break;
    case Runtime:
        AIMMBasic::DrawRuntime();
        break;
    case Flatbuffers:
        AIMMBasic::DrawFlatbuffers();
        break;
    case Location:
        AIMMBasic::DrawLocation();
        break;
    case WaterDetection:
        AIMMBasic::DrawWaterDetection();
        break;
    }

    ImGui::End();
}

void AIMMBasic::DrawAll() {

}

void AIMMBasic::DrawPeripherals()
{
    ImGui::SeparatorText("Peripheral Information");
    auto [passes, warnings, errors, operationStatus] = StatusLogger::getInstance()->getStatus("peripherals");
    AIMMBasic::TestStatusColumnsAndOverallHealth(passes, warnings, errors, operationStatus);

    AIMMBasic::RenderTestRow("peripherals");
}

void AIMMBasic::DrawStartup() {

    ImGui::SeparatorText("Startup Information");
    auto [passes, warnings, errors, operationStatus] = StatusLogger::getInstance()->getStatus("startup");
    AIMMBasic::TestStatusColumnsAndOverallHealth(passes, warnings, errors, operationStatus);

    AIMMBasic::RenderTestRow("startup");
}

void AIMMBasic::DrawRuntime() {

    ImGui::SeparatorText("Runtime Information");
    auto [passes, warnings, errors, operationStatus] = StatusLogger::getInstance()->getStatus("runtime");
    AIMMBasic::TestStatusColumnsAndOverallHealth(passes, warnings, errors, operationStatus);

    AIMMBasic::RenderTestRow("runtime");
}

void AIMMBasic::DrawFlatbuffers() {

    ImGui::SeparatorText("Flatbuffers Information");
    auto [passes, warnings, errors, operationStatus] = StatusLogger::getInstance()->getStatus("flatbuffers");
    AIMMBasic::TestStatusColumnsAndOverallHealth(passes, warnings, errors, operationStatus);

    AIMMBasic::RenderTestRow("flatbuffers");
}

void AIMMBasic::DrawRadioFrequency()
{
    ImGui::SeparatorText("Radio Frequency Information");
    auto [passes, warnings, errors, operationStatus] = StatusLogger::getInstance()->getStatus("radio_frequency");
    AIMMBasic::TestStatusColumnsAndOverallHealth(passes, warnings, errors, operationStatus);

    AIMMBasic::RenderTestRow("radio_frequency");
}

void AIMMBasic::DrawLocation() {

}

void AIMMBasic::DrawWaterDetection() {

}

void AIMMBasic::DrawGeneralMenu()
{

    auto [passes, warnings, errors, operationStatus] = StatusLogger::getInstance()->getStatus("overview");
    AIMMBasic::TestStatusColumnsAndOverallHealth(passes, warnings, errors, operationStatus);

    ImGui::Separator(); // Add a separator after the headers for clarity

    AIMMBasic::RenderTestRow("overview");

}

void AIMMBasic::DrawPowerMenu()
{
    // Content for Power Settings
    ImGui::Text("Power Management and Settings");
}

/**
 * @brief The below functions are helper functions to the above
 * 
 */

