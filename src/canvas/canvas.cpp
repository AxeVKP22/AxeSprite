#include "canvas.hpp"

float myColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};

std::vector<Color> pixels;
bool initialized = false;
Texture2D texture;

float zoom = 1.0f;

ImVec2 resolution;

void imGuiRenderCanvasWindow(const char* windowName) {
    for (int i = 0; i < canvasNames.size(); i++) {
        if (!initialized) {
            if (isOpenSelected) {
                if (texture.id > 0) UnloadTexture(texture);
                texture = LoadTexture(pathToFile);
            } 

            else if (isNewSelected) {
                if (texture.id > 0) UnloadTexture(texture);
                
                Image img = GenImageColor(newWidth, newHeight, WHITE);
                texture = LoadTextureFromImage(img);
                UnloadImage(img);
            }

            initialized = true;
        }
        ImGui::Begin((std::string("Canvas: ") + canvasNames[i]).c_str(), nullptr);

        ImGui::BeginChild("file", ImVec2(150, 30));
        ImGui::SmallButton("File");
        ImGui::EndChild();

        ImGui::BeginChild("color", ImVec2(300, 300));
        ImGui::ColorPicker4("Color", myColor);
        ImGui::EndChild();

        float mouse = ImGui::GetIO().MouseWheel;
        if (!ImGui::IsWindowHovered() && mouse != 0.0f) {
            zoom += (mouse + 0.1f);
            if (zoom < 0.1f) {              
                zoom = 0.1f;
            }
        }

        ImGui::SameLine();
    
        ImGui::BeginChild("Pixel Window", ImVec2(0, 0), true);
        ImVec2 resolution = ImGui::GetContentRegionAvail();

        ImVec2 textureSize = ImVec2(texture.width * zoom, texture.height * zoom);

        ImVec2 texturePos;
        texturePos.x = std::max((resolution.x - textureSize.x) * 0.5f, 0.0f);
        texturePos.y = std::max((resolution.y - textureSize.y) * 0.5f, 0.0f);

        ImGui::SetCursorPos(texturePos);

        ImGui::Image((void*)(intptr_t)texture.id, textureSize);
        ImGui::EndChild();

        ImGui::End();
    }
}