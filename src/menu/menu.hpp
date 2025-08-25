#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include "../globals.hpp"

#define MAX_PATH_LENGHT 260

extern bool isFileSubMenuOpen;
extern bool isNewPressed;
extern bool isOpenPressed;

extern char pathToFile[MAX_PATH_LENGHT];

extern int newWidth;
extern int newheight;
extern char newName[32];

extern std::vector<std::string> canvasNames;

void imGuiRenderMenuWindow(GLFWwindow* window, const char* windowName);
void imGuiRenderFileSubMenu();
void imGuiRenderNewSubMenu();
int imGuiRenderOpenSubMenu();