#include <iostream>
#include "init/imgui.hpp"
#include "menu/menu.hpp"

int main() {

    GLFWwindow* window = glfwCreateWndw(800,  800, "AxeSprite");

    imGuiInit(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        imGuiRenderMenuWindow(window, "Menu");
    }

    imGuiShutdown(window);

    return 0;
}
