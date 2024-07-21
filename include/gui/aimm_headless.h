#pragma once

#include <cstdint> // For uint8_t
#include "imgui.h" // Ensure ImGui and related libraries are included before this header if needed elsewhere

#include "runner.h"
#include <chrono>
#include <thread>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuiNotify.h"
#include "IconsFontAwesome6.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "runner.h"
#include "console.h"
#include "logger.h"

namespace AIMMNoBackend
{

    // Enum for managing AIMM state
    enum eAIMMState : uint8_t
    {
        Start,
        Disconnected,
        Connected,
    };

    // Function prototypes for initializing, managing connection, and rendering ImGui content
    bool Client_Startup();
    void Client_Shutdown();
    void Client_Connect(eAIMMState &AIMMState, quill::Logger logger);
    void Client_Draw(bool &bQuit, AIMMClient_Base &AIMM);

} // namespace AIMMNoBackend

// Prototype for headless function
int headless(AIMMClient_Base &AIMM, quill::Logger *logger);
