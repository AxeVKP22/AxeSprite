#pragma once
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

void rlCreateWindow(int width, int height, const char* title);
void rlShutdownWindow(GLFWwindow* window);

