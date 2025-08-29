#include "canvas.hpp"

float myColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
std::vector<uint32_t> pixel;

void imGuiRenderCanvasWindow(const char* windowName) {
    for (int i = 0; i < canvasNames.size(); i++) {
        ImGui::Begin((std::string("Canvas: ") + canvasNames[i]).c_str(), nullptr);

        ImGui::BeginChild("file", ImVec2(150, 30));
        ImGui::SmallButton("File");
        ImGui::EndChild();

        ImGui::BeginChild("color", ImVec2(300, 300));
        ImGui::ColorPicker4("Color", myColor);
        ImGui::EndChild();

        ImGui::End();
    }
}