#ifndef NETIMGUI_SERVER_REMOTECLIENT_H
#define NETIMGUI_SERVER_REMOTECLIENT_H

#include <cstdint>

static void glfw_error_callback(int error, const char *description);

void ExtraSampleBackend_UpdateFontTexture();

void TextureCreate(const uint8_t *pPixelData, uint32_t width, uint32_t height, void *&pTextureViewOut);

void TextureDestroy(void *&pTextureView);

extern float g_MonitorDPIScale;

int entrance();

#endif // NETIMGUI_SERVER_REMOTECLIENT_H
