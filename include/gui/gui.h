#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "NetImgui_Api.h"
#include "Sample.h"
#include "NetImgui_Config.h"
#include "logger.h"

#include <stdio.h>
#define GL_SILENCE_DEPRECATION

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// Forward declarations to reduce dependencies
class SampleBasic; // Forward declare SampleBasic if used in other files

// GUI Starter
void gui(quill::Logger *logger);

// Initialize GLFW library and error handling
void initGLFW(quill::Logger *logger);

// Set up the GLFW window and the OpenGL context
GLFWwindow *setupWindow(quill::Logger *logger, const char *&glsl_version);

// Set up Dear ImGui contexts, configurations, and initializations
void setupImGui(GLFWwindow *window, const char *glsl_version);

// Main rendering loop
void mainGuiLoop(GLFWwindow *window, SampleBasic &sample);

    // Render a single frame
    void renderFrame(GLFWwindow *window, SampleBasic &sample); // Modified to include the SampleBasic reference

// Clean up and destroy ImGui, GLFW, and OpenGL contexts
void cleanupGLFW(GLFWwindow *window);

#endif // GUI_MANAGER_H
