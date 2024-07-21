#include "aimm_headless.h"

namespace AIMMNoBackend
{

    //=================================================================================================
    // Initialize the Dear Imgui Context and the NetImgui library
    //=================================================================================================
    bool Client_Startup()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();

        io.BackendFlags |= ImGuiBackendFlags_HasGamepad; // Enable NetImgui Gamepad support

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

        io.Fonts->AddFontDefault();

        float baseFontSize = 16.0f;                      // Default font size
        float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

        std::ifstream fontAwesomeFile(FONT_ICON_FILE_NAME_FAS);

        if (!fontAwesomeFile.good())
        {
            // // If it's not good, then we can't find the font and should abort
            // std::cerr << "Could not find the FontAwesome font file." << std::endl;
            abort();
        }

        static const ImWchar iconsRanges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
        ImFontConfig iconsConfig;
        iconsConfig.MergeMode = true;
        iconsConfig.PixelSnapH = true;
        iconsConfig.GlyphMinAdvanceX = iconFontSize;
        io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, iconFontSize, &iconsConfig, iconsRanges);

        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        io.Fonts->Build();
        io.Fonts->SetTexID(0);
        io.DisplaySize = ImVec2(8, 8);
        ImGui::StyleColorsDark();

        if (!NetImgui::Startup())
            return false;

        return true;
    }

    //=================================================================================================
    // Release resources
    //=================================================================================================
    void Client_Shutdown()
    {
        NetImgui::Shutdown();
        ImGui::DestroyContext(ImGui::GetCurrentContext());
    }

    //=================================================================================================
    // Manage connection to NetImguiServer
    //=================================================================================================
    void Client_Connect(eAIMMState &AIMMState, quill::Logger *logger)
    {
        constexpr char zClientName[] = "AIMM GUI";
        if (AIMMState == eAIMMState::Start)
        {
            LOG_TRACE_L2(logger, "Connecting to NetImguiServer to (127.0.0.1:8888)... ");
            NetImgui::ConnectToApp(zClientName, "localhost");
            while (NetImgui::IsConnectionPending())
                ;
            bool bSuccess = NetImgui::IsConnected();
            AIMMState = bSuccess ? eAIMMState::Connected : eAIMMState::Disconnected;
            if (!bSuccess)
            {
                LOG_TRACE_L2(logger, "Waiting for a connection from NetImguiServer on port 8889... ");
                NetImgui::ConnectFromApp(zClientName);
            }
        }
        else if (AIMMState == eAIMMState::Disconnected && NetImgui::IsConnected())
        {
            AIMMState = eAIMMState::Connected;
            LOG_TRACE_L1(logger, "Connected to client!");
        }
        else if (AIMMState == eAIMMState::Connected && !NetImgui::IsConnected())
        {
            AIMMState = eAIMMState::Disconnected;
            LOG_TRACE_L1(logger, "Waiting for a connection from NetImguiServer on port 8889... ");
            NetImgui::ConnectFromApp(zClientName);
        }
    }

    //=================================================================================================
    // Render all of our Dear ImGui Content (when appropriate)
    //=================================================================================================
    void Client_Draw(bool &bQuit, AIMMClient_Base &AIMM)
    {
        AIMM.Draw();
    }

} // namespace AIMMNoBackend

int headless(AIMMClient_Base &AIMM, quill::Logger *logger)
{
    if (!AIMMNoBackend::Client_Startup())
    {
        LOG_ERROR(logger, "Failed to initialize backend GUI.");
        AIMMNoBackend::Client_Shutdown();
        return -1;
    }

    // Main loop
    bool bQuit = false;
    AIMMNoBackend::eAIMMState AIMMState = AIMMNoBackend::eAIMMState::Start;
    while (!bQuit)
    {
        // AIMM Update
        AIMMNoBackend::Client_Connect(AIMMState, logger);
        AIMMNoBackend::Client_Draw(bQuit, AIMM);
    }

    // Cleanup
    AIMMNoBackend::Client_Shutdown();
    return 0;
}