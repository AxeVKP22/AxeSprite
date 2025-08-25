#include <iostream>
#include "globals.hpp"
#include "init/imgui.hpp"
#include "menu/menu.hpp"
#include "canvas/canvas.hpp"

int main() {

    GLFWwindow* window = glfwCreateWndw(800,  800, "AxeSprite");

    imGuiInit(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();  

        imGuiRenderMenuWindow(window, "Menu");

        if (!canvasNames.empty()) {
            imGuiRenderCanvasWindow(window, "Canvas");
        }


        ImGui::Render();
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    imGuiShutdown(window);

    return 0;
}
