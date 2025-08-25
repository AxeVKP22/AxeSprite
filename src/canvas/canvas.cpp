#include "canvas.hpp"

void imGuiRenderCanvasWindow(GLFWwindow* window, const char* windowName) {                                                        
    for (int i = 0; i < canvasNames.size(); i++) {
        ImGui::Begin(
            (std::string("Canvas: ") + canvasNames[i]).c_str(),
            nullptr
        );
        ImGui::End();
    }

}       