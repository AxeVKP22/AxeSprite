#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <stb_image.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "../menu/menu.hpp"

extern std::vector<uint32_t> px;
extern float myColor[4];

void imGuiRenderCanvasWindow(GLFWwindow* window, const char* windowName);