#pragma once 
#include <iostream>
#include <vector>
#include <cstdint>
#include <array>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "../menu/menu.hpp"

extern "C" {
    #include "raylib.h"
}




#define MAX_PATH_LENGHT 260

extern float myColor[4];
extern float transparent[4];
extern ImGuiWindowFlags windowFlags;

extern float zoom;

void imGuiRenderCanvasWindow(const char* windowName);

void imGuiSaveAs(Image image);