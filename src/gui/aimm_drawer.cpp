#include <NetImgui_Api.h>
#include "runner.h"
#include "ImGuiNotify.h"
#include "IconsFontAwesome6.h"

//=================================================================================================
// AIMM GUI CLASS
//=================================================================================================
class AIMMBasic : public AIMMClient_Base
{
public:
	AIMMBasic() : AIMMClient_Base("Notre Dame - AIMM Team GUI") {}
	virtual ImDrawData *Draw() override;
	void DrawSettingsWindow(bool *p_open);
	void DrawStatusWindow(bool *s_open);
	void TestStatusButton(const char *id, const ImVec4 &color, bool running, int display_counter);

		private : int colorIndex = 0; // To cycle through colors
	std::vector<ImVec4> colors = {
		ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // Red
		ImVec4(0.0f, 1.0f, 0.0f, 1.0f), // Green
		ImVec4(1.0f, 1.0f, 0.0f, 1.0f)	// Yellow
	};
};

//=================================================================================================
// GET AIMM
// Each project must return a valid AIMM object
//=================================================================================================
AIMMClient_Base &GetAIMM()
{
	static AIMMBasic AIMM;
	return AIMM;
}

//=================================================================================================
// DRAW - Most Things Should be Drawn Here
//=================================================================================================
ImDrawData *AIMMBasic::Draw()
{
	//---------------------------------------------------------------------------------------------
	// (1) Start a new Frame.
	// Note:	With ImGui 1.81+ NetImgui can automatically intercept Imgui::NewFrame/Render. This
	//			AIMM does this. For older Imgui releases, please look at 'Client_Draw_ModeAlways'
	//			in 'AIMMNewFrame' on how to tell NetImgui directly about NewFrame/EndFrame.
	//			Other AIMMs also avoid the auto intercept to allow drawing only when needed.
	//---------------------------------------------------------------------------------------------
	ImGui::NewFrame();

	//-----------------------------------------------------------------------------------------
	// (2) Draw ImGui Content
	//-----------------------------------------------------------------------------------------
	auto drawFlags = AIMMClient_Base::Draw_Connect();

	if (drawFlags[DrawFlags::ShowSettingsWindow])
	{
		DrawSettingsWindow(&drawFlags[DrawFlags::ShowSettingsWindow]);
	}

	if (drawFlags[DrawFlags::ShowApplicationWindow])
	{
	}

	if (drawFlags[DrawFlags::ShowTerminalWindow])
	{
	}

	if (drawFlags[DrawFlags::ShowStatusWindow])
	{
		DrawStatusWindow(&drawFlags[DrawFlags::ShowSettingsWindow]);
	}

	//---------------------------------------------------------------------------------------------
	// (3) Finish the frame
	// Note:	Same note as in (1)
	//---------------------------------------------------------------------------------------------
	ImGui::Render();

	//---------------------------------------------------------------------------------------------
	// (4) Return content to draw by local renderer. Stop drawing locally when remote connected
	//---------------------------------------------------------------------------------------------
	return !NetImgui::IsConnected() ? ImGui::GetDrawData() : nullptr;
}

void AIMMBasic::DrawSettingsWindow(bool *p_open)
{

	if (!ImGui::Begin("Settings", p_open))
	{
		ImGui::End();
		return;
	}


	ImGui::Text("ND-AIMM Main Computer GUI - V1.0.0");

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

		ImGui::Columns(3, "statusColumns"); // Create three columns
		ImGui::SetColumnWidth(0, 50);		// Set width for the color square column
		ImGui::SetColumnWidth(1, 300);		// Set width for the description column
		ImGui::SetColumnWidth(2, 100);		// Set width for the status column

		// Use a solid block character or rectangle to ensure visibility
		TestStatusButton("colorButton1", colors[colorIndex], true, -1);
		ImGui::NextColumn();
		ImGui::Text("Test Description"); // Display description
		ImGui::NextColumn();
		ImGui::Text("PASS"); // Display status
		ImGui::NextColumn();

		ImGui::Columns(1); // Reset to one column

		ImGui::End();
	}

void AIMMBasic::TestStatusButton(const char *id, const ImVec4 &color, bool running, int display_counter)
{
	// ImGui::PushItemFlag(ImGuiItemFlags_NoTabStop, true);
	ImGui::ColorButton(id, color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder);
	// ImGui::PopItemFlag();

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

	// // Note: Some dummy text content
	// ImGui::SetNextWindowPos(ImVec2(32, 48), ImGuiCond_Once);
	// ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Once);
	// if (ImGui::Begin("AIMM Basic", nullptr))
	// {
	// 	ImGui::TextColored(ImVec4(0.1, 1, 0.1, 1), "Basic demonstration of NetImgui code integration.");
	// 	ImGui::TextWrapped("Create a basic Window with some text.");
	// 	ImGui::NewLine();
	// 	ImGui::TextColored(ImVec4(0.1, 1, 0.1, 1), "Where are we drawing: ");
	// 	ImGui::SameLine();
	// 	ImGui::TextUnformatted(NetImgui::IsDrawingRemote() ? "Remote Draw" : "Local Draw");
	// 	ImGui::NewLine();
	// 	ImGui::TextColored(ImVec4(0.1, 1, 0.1, 1), "Filler content");
	// 	ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
	// }
	// ImGui::End();

	// if (ImGui::Begin("AIMM Basic Another", nullptr))
	// {
	// 	ImGui::TextColored(ImVec4(0.1, 1, 0.1, 1), "Basic demonstration of NetImgui code integration.");
	// 	ImGui::TextWrapped("Create a basic Window with some text.");
	// 	ImGui::NewLine();
	// 	ImGui::TextColored(ImVec4(0.1, 1, 0.1, 1), "Where are we drawing: ");
	// 	ImGui::SameLine();
	// 	ImGui::TextUnformatted(NetImgui::IsDrawingRemote() ? "Remote Draw" : "Local Draw");
	// 	ImGui::NewLine();
	// 	ImGui::TextColored(ImVec4(0.1, 1, 0.1, 1), "Filler content");
	// 	ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
	// }
	// ImGui::End();