#include "runner.h"
#include "console.h"


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
		// Implement Application Window
	}

	if (drawFlags[DrawFlags::ShowTerminalWindow])
	{
		showAIMMConsole(&drawFlags[DrawFlags::ShowTerminalWindow]);
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