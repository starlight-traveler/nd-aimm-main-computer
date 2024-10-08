// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp


// Do not change order of includes
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
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "thread_safe_gui_status_logger.hpp"
#include "aimm_test_manager.h"
#include "aimm_test_detail.h"
#include "aimm_headless.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// (DPI Awareness)
// Function added to original backend code, handling Font Texture updates
// when required (like a DPI change) in various AIMMs
// In this backend, we do not handle this.
// See Code/ThirdParty/DearImgui/backends/imgui_impl_dx11.cpp for an example of how to handle this.
void ExtraAIMMBackend_UpdateFontTexture()
{
    //    ImGui_ImplDX11_Data* bd = ImGui_ImplDX11_GetBackendData();
    //    if (bd && bd->pFontTextureView)
    //    {
    //        bd->pFontTextureView->Release();
    //        bd->pFontTextureView = nullptr;
    //        bd->pFontAIMMr->Release();
    //        bd->pFontAIMMr = nullptr;
    //        ImGui::GetIO().Fonts->SetTexID(0); // We copied data->pFontTextureView to io.Fonts->TexID so let's clear that as well.
    //    }
    //    ImGui_ImplDX11_CreateFontsTexture();
}

// Avoids adding DirectX11 dependencies in ClientAIMM, with all the disable warning required
void TextureCreate(const uint8_t *pPixelData, uint32_t width, uint32_t height, void *&pTextureViewOut)
{
    //    D3D11_TEXTURE2D_DESC desc;
    //    D3D11_SUBRESOURCE_DATA subResource;
    //
    //    ZeroMemory(&desc, sizeof(desc));
    //    desc.Width = static_cast<UINT>(width);
    //    desc.Height = static_cast<UINT>(height);
    //    desc.MipLevels = 1;
    //    desc.ArraySize = 1;
    //    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //    desc.AIMMDesc.Count = 1;
    //    desc.Usage = D3D11_USAGE_DEFAULT;
    //    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    //    desc.CPUAccessFlags = 0;
    //
    //    ID3D11Texture2D* pTexture = nullptr;
    //    subResource.pSysMem = pPixelData;
    //    subResource.SysMemPitch = desc.Width * 4;
    //    subResource.SysMemSlicePitch = 0;
    //    g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);
    //
    //    if( pTexture )
    //    {
    //        // Create texture view
    //        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    //        ZeroMemory(&srvDesc, sizeof(srvDesc));
    //        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    //        srvDesc.Texture2D.MipLevels = desc.MipLevels;
    //        srvDesc.Texture2D.MostDetailedMip = 0;
    //        g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, reinterpret_cast<ID3D11ShaderResourceView**>(&pTextureViewOut));
    //
    //        pTexture->Release();
    //    }
}


// Avoids adding DirectX11 dependencies in ClientAIMM, with all the disable warning required
void TextureDestroy(void *&pTextureView)
{
    //    if (pTextureView)
    //    {
    //        reinterpret_cast<ID3D11ShaderResourceView*>(pTextureView)->Release();
    //        pTextureView = nullptr;
    //    }
}

float g_MonitorDPIScale = 0; // (DPI Awareness)

// Main code
int gui(quill::Logger *logger)
{

    AIMMClient_Base &AIMM = GetAIMM();

#ifdef HEADLESS

    headless(AIMM, logger);

    return 0;

#else

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
    #if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100
        const char *glsl_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    #elif defined(__APPLE__)
        // GL 3.2 + GLSL 150
        const char *glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
    #else
        // GL 3.0 + GLSL 130
        const char *glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    #endif


    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(1280, 720, "AIMM GUI", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    io.Fonts->AddFontDefault();

    float baseFontSize = 16.0f;                      // Default font size
    float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

    // Check if FONT_ICON_FILE_NAME_FAS is a valid path
    std::ifstream fontAwesomeFile(FONT_ICON_FILE_NAME_FAS);

    if (!fontAwesomeFile.good())
    {
        // If it's not good, then we can't find the font and should abort
        std::cerr << "Could not find the FontAwesome font file." << std::endl;
        abort();
    }

    static const ImWchar iconsRanges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
    ImFontConfig iconsConfig;
    iconsConfig.MergeMode = true;
    iconsConfig.PixelSnapH = true;
    iconsConfig.GlyphMinAdvanceX = iconFontSize;
    io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, iconFontSize, &iconsConfig, iconsRanges);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    io.Fonts->AddFontDefault();

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    /**
     * @brief Startup for the AIMM GUI Section
     * 
     */
    bool startupSuccess = AIMM.Startup();
    if (!startupSuccess)
    {
        AIMM.Shutdown();
        return 1;
    }

    /**
     * @brief Initalize all systems to start looking for systems
     * 
     */

    /**
     * @brief Entrace Point for the Entire GUI Loop
     * 
     */

    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        AIMM.UpdateFont(g_MonitorDPIScale, true);

        IM_UNUSED(show_demo_window);
        IM_UNUSED(show_another_window);

        // Avoids high CPU/GPU usage by releasing this thread until enough time has passed
        static auto sLastTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedSec = std::chrono::high_resolution_clock::now() - sLastTime;
        if (elapsedSec.count() < 1.f / 120.f)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(250));
            continue;
        }

        // Clear the screen
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        /**
         * @brief All GUI platforms are called here
         * 
         * You will likely need to call the class in this GUI section to draw
         * or combine it in one of the other classes (depends on what is being
         * drawn)
         */

        // Draw the Local Imgui UI and remote imgui UI
        sLastTime = std::chrono::high_resolution_clock::now();
        ImDrawData *pDrawData = AIMM.Draw();
        if (pDrawData)
        {
            ImGui_ImplOpenGL3_RenderDrawData(pDrawData);
        }

        /**
         * @brief Rendering platform
         * 
         * Do not touch.
         */

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        static int sLastFrame = -1;
        int newFrame = ImGui::GetFrameCount();
        if (sLastFrame != newFrame && io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;

#endif
}