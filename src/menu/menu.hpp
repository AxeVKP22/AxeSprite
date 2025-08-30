#pragma once 
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <iostream>
#include <vector>
#include "../globals.hpp"

#define MAX_PATH_LENGHT 260

extern bool isFileSubMenuOpen;
extern bool isNewPressed;
extern bool isOpenPressed;
extern bool isNewSelected;
extern bool isOpenSelected;

extern char pathToFile[MAX_PATH_LENGHT];
extern char newName[32];

extern int newWidth;
extern int newHeight;

extern std::vector<std::string> canvasNames;

struct returnOpenCode
{
    bool success;
    std::string filename;
};

void imGuiRenderMenuWindow(const char* windowName);
void imGuiRenderFileSubMenu();
void imGuiRenderNewSubMenu();
returnOpenCode imGuiRenderOpenSubMenu();