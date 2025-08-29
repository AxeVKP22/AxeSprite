#include <iostream>
#include <vector>
#include <cstdint>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "../menu/menu.hpp"

extern std::vector<uint32_t> pixel;
extern float myColor[4];

void imGuiRenderCanvasWindow(const char* windowName);