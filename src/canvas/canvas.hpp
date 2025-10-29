#pragma once 
#include <iostream>
#include <vector>
#include <cstdint>
#include <array>
#include <cmath>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "../menu/menu.hpp"
#include "save.hpp"
#include "../init/imgui.hpp"

extern "C" {
    #include "raylib.h"
}

#define MAX_PATH_LENGHT 260

extern float myColor[4];
extern float transparent[4];
extern float zoom;

extern ImGuiWindowFlags windowFlags;
extern bool blockMove;

extern bool NisFileSubMenuOpen;

extern int drawingMode;
extern ImVec2 startPos;
extern ImVec2 finishPos;


void imGuiRenderCanvasWindow(const char* windowName);
void NimGuiRenderFileSubMenu();
void drawLine(int x0, int y0, int x1, int y1);
void drawCircle(int xm, int ym, int r);
void drawPixel(const float* color, int x, int y);
void createImage();
void drawColorUI();
void canvasWindow();
void drawPixelGrid(ImVec2 pos, ImVec2 size, Texture2D tex, float zm);