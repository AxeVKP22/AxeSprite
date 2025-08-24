#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <windows.h>
#include <iostream>

void imGuiRenderMenuWindow(GLFWwindow* window, const char* windowName);
void imGuiRenderFileSubMenu();
void imGuiRenderNewSubMenu();
int imGuiRenderOpenSubMenu();