#pragma once

#include <NetImgui_Api.h>
#include <map>
#include <string>
#include <vector>
#include "ImGuiNotify.h"
#include "IconsFontAwesome6.h"

// Forward declares when NetImgui is not enabled
#if !NETIMGUI_ENABLED
struct ImGuiContext;
struct ImDrawData;
namespace NetImgui
{
	using ThreadFunctPtr = void *;
	using FontCreateFuncPtr = void *;
}
#endif

enum class DrawFlags
{
	ShowSettingsWindow,
	ShowApplicationWindow,
	ShowTerminalWindow,
	ShowStatusWindow
};

class AIMMClient_Base
{
public:
	AIMMClient_Base(const char *AIMMName);				   //!< Constructor receiving pointer to constant string that must remains valid
	virtual bool Startup();									   //!< Called once when starting
	virtual void Shutdown();								   //!< Called once when exiting
	virtual bool UpdateFont(float fontScaleDPI, bool isLocal); //!< Receive command to create/update the Font Atlas and its texture data
	virtual ImDrawData *Draw() = 0;							   //!< Each AIMM should have their Dear ImGui drawing routines in this overloaded method

private:

protected:
	std::map<DrawFlags, bool> Draw_Connect(); //!< Display UI for initiating a connection to the remote NetImgui server application
	const char *mAIMMName = nullptr;						  //!< Name displayed in the Main Menu bar (must receive string pointer in constructor that remains valid)
	ImGuiContext *mpContextMain = nullptr;						  //!< Pointer to main context created in main.cpp (used to detect when to update font texture)
	ImGuiContext *mpContextLocal = nullptr;						  //!< Pointer to context used for local draw. Most AIMM leave it to the same as mpContextMain (used to detect when to update font texture)
	NetImgui::ThreadFunctPtr mCallback_ThreadLaunch = nullptr;	  //!< [Optional] Thread launcher callback assigned on NetImgui connection. Used to start a new thread for coms with NetImgui server
	NetImgui::FontCreateFuncPtr mCallback_FontGenerate = nullptr; //!< [Optional] Font generation callback assigned on NetImgui connection. Used to adjust the font data to remote server DPI
	float mGeneratedFontScaleDPI = 0.f;							  //!< Current generated font texture DPI
	bool mbShowDemoWindow = false;								  //!< If we should show the Dear ImGui demo window
	bool showSettingsWindow = false;							      
	bool showApplicationWindow = false;
	bool showTerminalWindow = false;
	bool showStatusWindow = false;
	char mConnect_HostnameServer[128] = {"localhost"};			  //!< IP/Hostname used to send a connection request when when trying to reach the server
	int mConnect_PortServer = 0;								  //!< Port used to send a connection request when when trying to reach the server
	int mConnect_PortClient = 0;								  //!< Port opened when waiting for a server connection request
};

//=================================================================================================
// AIMM GUI CLASS
//=================================================================================================
class AIMMBasic : public AIMMClient_Base
{
public:
	AIMMBasic() : AIMMClient_Base("Notre Dame - AIMM Team GUI") {}

	// Overriden Functions
	virtual ImDrawData *Draw() override;

	// Settings Functions
	void DrawSettingsWindow(bool *p_open);

	// Status Functions
	void DrawStatusWindow(bool *s_open);
	void TestStatusButton(const char *id, const ImVec4 &color, bool running, int display_counter);
	void TestStatusColumnsAndOverallHealth(int numPasses, int numWarnings, int numErrors, std::string operationalStatus);
	void RenderTestRow(std::string category);
	void DrawGeneralMenu();
	void DrawPowerMenu();
	void DrawAll();
	void DrawPeripherals();
	void DrawStartup();
	void DrawRuntime();
	void DrawFlatbuffers();
	void DrawLocation();
	void DrawWaterDetection();
	void DrawRadioFrequency();

	enum MenuType
	{
		General,
		Power,
		Peripherals,
		RadioFrequency,
		Startup,
		Runtime,
		Flatbuffers,
		Location,
		WaterDetection
	};

	MenuType currentMenu = General; // Default to general menu

private:
	int colorIndex = 0; // To cycle through colors
	std::vector<ImVec4> colors = {
		ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // Red
		ImVec4(0.0f, 1.0f, 0.0f, 1.0f), // Green
		ImVec4(1.0f, 1.0f, 0.0f, 1.0f)	// Yellow
	};
};

//=============================================================================
// The _s string functions are a mess. There's really no way to do this right
// in a cross-platform way. Best solution I've found is to just use strncpy,
// infer the buffer length, and null terminate. Still need to suppress
// the warning on Windows.
// See https://randomascii.wordpress.com/2013/04/03/stop-using-strncpy-already/
// and many other discussions online on the topic.
//=============================================================================
template <size_t charCount>
inline void StringCopy(char (&output)[charCount], const char *pSrc, size_t srcCharCount = 0xFFFFFFFE)
{
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4996) // warning C4996: 'strncpy': This function or variable may be unsafe.
#endif

	size_t charToCopyCount = charCount < srcCharCount + 1 ? charCount : srcCharCount + 1;
	strncpy(output, pSrc, charToCopyCount - 1);
	output[charCount - 1] = 0;

#if defined(_MSC_VER) && defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif
}

AIMMClient_Base &GetAIMM(); // Each AIMM must implement this function and return a valid AIMM object

#include "NetImgui_WarningDisable.h"
#include "NetImgui_WarningDisable.h"
