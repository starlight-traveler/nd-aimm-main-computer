#include "runner.h"

void AIMMBasic::DrawSettingsWindow(bool *p_open)
{

    if (!ImGui::Begin("Settings", p_open))
    {
        ImGui::End();
        return;
    }

    ImGui::Text("ND-AIMM Main Computer GUI - V1.0.3");

    ImGui::TextLinkOpenURL("Homepage", "https://github.com/ocornut/imgui");
    ImGui::SameLine();
    ImGui::TextLinkOpenURL("FAQ", "https://github.com/ocornut/imgui/blob/master/docs/FAQ.md");
    ImGui::SameLine();
    ImGui::TextLinkOpenURL("Wiki", "https://github.com/ocornut/imgui/wiki");
    ImGui::SameLine();
    ImGui::TextLinkOpenURL("Releases", "https://github.com/ocornut/imgui/releases");
    ImGui::SameLine();
    ImGui::TextLinkOpenURL("Funding", "https://github.com/ocornut/imgui/wiki/Funding");

    ImGui::Separator();
    ImGui::Text("By: The Notre Dame AIMM Team");
    ImGui::Text("ND-AIMM's derivative works are under the MIT License, see LICENSE for more information.");
    ImGui::Separator();

    if (ImGui::Button(" Show ImGui Demo "))
    {
        mbShowDemoWindow = !mbShowDemoWindow;
    }

    if (ImGui::Button("Success"))
    {
        ImGuiToast toast(ImGuiToastType::Success, 3000); // <-- content can also be passed here as above
        toast.setTitle("This is a %s title", "wonderful");
        toast.setContent("Lorem ipsum dolor sit amet");
        ImGui::InsertNotification(toast);
    }

    ImGui::End();
}