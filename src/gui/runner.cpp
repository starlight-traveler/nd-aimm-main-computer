#include <NetImgui_Api.h>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include "runner.h"
#include "resources/fonts/roboto_medium.cpp"
#include "ImGuiNotify.h"

//=================================================================================================
// FontCreationCallback_Default
//-------------------------------------------------------------------------------------------------
// Default handling of remote server request to adjust the font DPI
// This is optional, when no callback is specified on connection function, we use
// 'ImGui::GetIO().FontGlobalScale' to adjust the font display size. Simple but blurier results.
//=================================================================================================
void FontCreationCallback_Default(float PreviousDPIScale, float NewDPIScale)
{
	IM_UNUSED(PreviousDPIScale); IM_UNUSED(NewDPIScale);
#if NETIMGUI_ENABLED
	if (GetAIMM().UpdateFont(NewDPIScale, false))
	{
		uint8_t* pPixelData(nullptr); int width(0), height(0);
		ImGui::GetIO().Fonts->GetTexDataAsAlpha8(&pPixelData, &width, &height);
		NetImgui::SendDataTexture(ImGui::GetIO().Fonts->TexID, pPixelData, static_cast<uint16_t>(width), static_cast<uint16_t>(height), NetImgui::eTexFormat::kTexFmtA8);
	}
#endif
}

//=================================================================================================
// Constructor
//-------------------------------------------------------------------------------------------------
//
//=================================================================================================
AIMMClient_Base::AIMMClient_Base(const char* AIMMName)
: mAIMMName(AIMMName)
{
#if NETIMGUI_ENABLED
	mConnect_PortClient		= NetImgui::kDefaultClientPort;
	mConnect_PortServer		= NetImgui::kDefaultServerPort;
	mCallback_FontGenerate	= FontCreationCallback_Default;
#endif
}

//=================================================================================================
// Startup
//-------------------------------------------------------------------------------------------------
//
//=================================================================================================
bool AIMMClient_Base::Startup()
{
	mpContextLocal = mpContextMain = ImGui::GetCurrentContext();
#if NETIMGUI_ENABLED
	if( !NetImgui::Startup() )
		return false;
#endif
	return true;
}

//=================================================================================================
// Shutdown
//-------------------------------------------------------------------------------------------------
//
//=================================================================================================
void AIMMClient_Base::Shutdown()
{
#if NETIMGUI_ENABLED
	NetImgui::Shutdown();
#endif
}

//=================================================================================================
// UpdateFont
//-------------------------------------------------------------------------------------------------
// Called from the main function and by remote server, to regnerate our font texture
// with the appropriate character pixel size. This is to handle monitor with DPI scaling to make
// small text readable on high resolution screens. 
// 
// The DPI scaling can also be entirely ignored by generating the font texture once 
// to a fixed size, paired with 'ImGui::GetIO().FontGlobalScale' for the text size increase.
// However, this create blurier text. See 'AIMMFontDPI' for more details
//=================================================================================================
bool AIMMClient_Base::UpdateFont(float fontScaleDPI, bool isLocal)
{
	IM_UNUSED(isLocal);
	constexpr float kFontPixelSize = 16.f;

#if NETIMGUI_ENABLED
	// Ignore local font resize when remotely drawing to this context
	bool isLocalFontWithRemoteContext = isLocal && NetImgui::IsConnected() && mpContextMain == ImGui::GetCurrentContext();
	if( !isLocalFontWithRemoteContext )
#endif
	{
		// We detect if the current scaling result in a different pixel size
		// When handling many font, each different font size would have to be tested
		int pixelSizeCurrent	= static_cast<int>(roundf(kFontPixelSize * mGeneratedFontScaleDPI));
		int pixelSizeWanted		= static_cast<int>(roundf(kFontPixelSize * fontScaleDPI));
		if(pixelSizeCurrent != pixelSizeWanted)
		{
			ImFontConfig FontConfig = {};
			ImFontAtlas* FontAtlas	= ImGui::GetIO().Fonts;
			mGeneratedFontScaleDPI	= fontScaleDPI;
			FontConfig.SizePixels	= static_cast<float>(pixelSizeWanted);
			FontAtlas->Clear();
		
		/**
		 * @brief Need to figure out why the callback for the font crashes the application, setting
		 * to default means it won't crash, but become unuseable on the system.
		 * 
		 */
		
			ImGuiIO &io = ImGui::GetIO();
			io.Fonts->AddFontDefault();

			float baseFontSize = 16.0f;						 
			float iconFontSize = baseFontSize * 2.0f / 3.0f;

			std::ifstream fontAwesomeFile(FONT_ICON_FILE_NAME_FAS);

			static const ImWchar iconsRanges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
			ImFontConfig iconsConfig;
			iconsConfig.MergeMode = true;
			iconsConfig.PixelSnapH = true;
			iconsConfig.GlyphMinAdvanceX = iconFontSize;
			io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, iconFontSize, &iconsConfig, iconsRanges);
			
			FontAtlas->Build();
			// Regenerate the Font Texture (only if used by local context)
			extern void ExtraAIMMBackend_UpdateFontTexture();
			if( ImGui::GetCurrentContext() == mpContextLocal ){
				ExtraAIMMBackend_UpdateFontTexture();
			}
			return true;
		}
	}
	return false;
}

//=================================================================================================
// Draw_Connect
//-------------------------------------------------------------------------------------------------
// Function called by all AIMMs, to display the Connection Options, and some other default
// MainMenu entries. 
//=================================================================================================
std::map<DrawFlags, bool> AIMMClient_Base::Draw_Connect()
{

	std::map<DrawFlags, bool> drawMap;

#if NETIMGUI_ENABLED
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3,6) );

	// Attempt to auto-connect to a predefined server at start if not already connected or attempting to connect
    if (!NetImgui::IsConnected() && !NetImgui::IsConnectionPending())
    {
        static bool autoAttempt = true;
        if (autoAttempt)
        {
            NetImgui::ConnectToApp("AIMM", "localhost", 8888, mCallback_ThreadLaunch, mCallback_FontGenerate);
            autoAttempt = false; // Only attempt once per application run
        }
    }


	if( ImGui::BeginMainMenuBar() )
	{				
		ImGui::AlignTextToFramePadding();
		ImGui::TextColored(ImVec4(0.1, 1, 0.1, 1), "%s", mAIMMName);
		ImGui::SameLine(0,32);
		
		//-----------------------------------------------------------------------------------------
		if( NetImgui::IsConnected() )
		//-----------------------------------------------------------------------------------------
		{
			ImGui::TextUnformatted("Status: Connected");
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));
			ImGui::SetCursorPosY(3);
			if( ImGui::Button(" Disconnect ") ) 
			{
				NetImgui::Disconnect();
			}
			ImGui::PopStyleVar();
		}
		
		//-----------------------------------------------------------------------------------------
		else if( NetImgui::IsConnectionPending() )
		//-----------------------------------------------------------------------------------------
		{
			ImGui::TextUnformatted("Status: Waiting Server");
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));
			ImGui::SetCursorPosY(3);
			if (ImGui::Button(" Cancel "))
			{
				NetImgui::Disconnect();
			}
			ImGui::PopStyleVar();
		}

		//-----------------------------------------------------------------------------------------
		else // No connection
		//-----------------------------------------------------------------------------------------
		{
			//-------------------------------------------------------------------------------------
			if( ImGui::BeginMenu("[ Connect To ]") )
			//-------------------------------------------------------------------------------------
			{
				ImGui::TextColored(ImVec4(0.1, 1, 0.1, 1), "Server Settings");
				ImGui::InputText("Hostname", mConnect_HostnameServer, sizeof(mConnect_HostnameServer));
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Address of PC running the netImgui server application. Can be an IP like 127.0.0.1");
				ImGui::InputInt("Port", &mConnect_PortServer);
				ImGui::NewLine();
				ImGui::Separator();
				if (ImGui::Button("Connect", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
				{
					NetImgui::ConnectToApp(mAIMMName, mConnect_HostnameServer, mConnect_PortServer, mCallback_ThreadLaunch, mCallback_FontGenerate);
				}
				ImGui::EndMenu();
			}

			if( ImGui::IsItemHovered() )
				ImGui::SetTooltip("Attempt a connection to a remote netImgui server at the provided address.");

			//-------------------------------------------------------------------------------------
			if (ImGui::BeginMenu("[  Wait For  ]"))
			//-------------------------------------------------------------------------------------
			{
				ImGui::TextColored(ImVec4(0.1, 1, 0.1, 1), "Client Settings");				
				ImGui::InputInt("Port", &mConnect_PortClient);
				ImGui::NewLine();
				ImGui::Separator();
				if (ImGui::Button("Listen", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
				{
					NetImgui::ConnectFromApp(mAIMMName, mConnect_PortClient, mCallback_ThreadLaunch, mCallback_FontGenerate);
				}
				ImGui::EndMenu();
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Start listening for a connection request by a remote netImgui server, on the provided Port.");
		}

		// ----------------
		
		// ImGui::SameLine(0,40);	
		// ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8,0.8,0.8,0.9) );
		// ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));				
		// ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, mbShowDemoWindow ? 1.f : 0.f);
		// ImGui::SetCursorPosY(3);
		// if( ImGui::Button(" Show ImGui Demo ") ){
		// 	mbShowDemoWindow = !mbShowDemoWindow;
		// }
		// ImGui::PopStyleColor();
		// ImGui::PopStyleVar(2);

		// ----------------


		// Button to toggle the visibility
		ImGui::SameLine(0,40);    
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8,0.8,0.8,0.9));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));                
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
		ImGui::SetCursorPosY(3);
		if (ImGui::Button(" Settings ")){
			showSettingsWindow = !showSettingsWindow; // Toggle the visibility of the info window
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		ImGui::SameLine(0, 40);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8, 0.8, 0.8, 0.9));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
		ImGui::SetCursorPosY(3);
		if (ImGui::Button(" Application "))
		{
			showSettingsWindow = !showSettingsWindow; // Toggle the visibility of the info window
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		ImGui::SameLine(0, 40);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8, 0.8, 0.8, 0.9));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
		ImGui::SetCursorPosY(3);
		if (ImGui::Button(" Terminal "))
		{
			showSettingsWindow = !showSettingsWindow; // Toggle the visibility of the info window
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		ImGui::SameLine(0, 40);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8, 0.8, 0.8, 0.9));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
		ImGui::SetCursorPosY(3);
		if (ImGui::Button(" Status "))
		{
			showStatusWindow = !showStatusWindow; // Toggle the visibility of the info window
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		ImGui::EndMainMenuBar();
	}

	ImGui::PopStyleVar();

	if( mbShowDemoWindow ){
		ImGui::ShowDemoWindow(&mbShowDemoWindow);
	}

	// ----------------------------------------------
	// All statements are below here for drawing
	// ----------------------------------------------

	// Notifications style setup
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);	  // Disable round borders
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f); // Disable borders

	// Notifications color setup
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.10f, 1.00f)); // Background color

	// Main rendering function
	ImGui::RenderNotifications();

	// ——————————————————————————————— WARNING ———————————————————————————————
	//  Argument MUST match the amount of ImGui::PushStyleVar() calls
	ImGui::PopStyleVar(2);
	// Argument MUST match the amount of ImGui::PushStyleColor() calls
	ImGui::PopStyleColor(1);

	drawMap[DrawFlags::ShowSettingsWindow] = showSettingsWindow;
	drawMap[DrawFlags::ShowApplicationWindow] = showSettingsWindow;
	drawMap[DrawFlags::ShowTerminalWindow] = showTerminalWindow;
	drawMap[DrawFlags::ShowStatusWindow] = showStatusWindow;

	return drawMap;

#endif // #if NETIMGUI_ENABLED
}