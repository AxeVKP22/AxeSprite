#include "canvas.hpp"

float myColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
std::vector<unsigned char> pixel(newWidth * newHeight * 4);
bool initialized = false;
Texture2D texture;

void imGuiRenderCanvasWindow(const char* windowName) {
    for (int i = 0; i < canvasNames.size(); i++) {
        if (!initialized) {
            texture = LoadTexture(pathToFile);
            initialized = true;
        }   
        ImGui::Begin((std::string("Canvas: ") + canvasNames[i]).c_str(), nullptr);

        ImGui::BeginChild("file", ImVec2(150, 30));
        ImGui::SmallButton("File");
        ImGui::EndChild();

        ImGui::BeginChild("color", ImVec2(300, 300));
        ImGui::ColorPicker4("Color", myColor);
        ImGui::EndChild();

        ImGui::SameLine();
        
        ImGui::BeginChild("Pixel Window", ImVec2(texture.width, texture.height));
        ImGui::Image((void*)(intptr_t)texture.id, ImVec2(texture.width, texture.height));
        ImGui::EndChild();

        ImGui::End();
    }
}

void renderTexture() {
    if (!initialized) {
        texture = LoadTexture(pathToFile);
        initialized = true;
    }

    ImGui::Begin("Pixel Window");
    ImGui::Image((void*)(intptr_t)texture.id, ImVec2(texture.width, texture.height));
    ImGui::End();
}
    