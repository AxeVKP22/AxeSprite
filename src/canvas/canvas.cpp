#include "canvas.hpp"
#include <vector>
#include "stb_image.h"

float myColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};

static GLuint texture = 0;
static int width = 0, height = 0;

std::vector<uint32_t> pixel;

void imGuiRenderCanvasWindow(GLFWwindow* window, const char* windowName) {
    static bool loaded = false;
    
    if (!loaded) {
        int numChannels;
        unsigned char* bytes = stbi_load("test/image.png", &width, &height, &numChannels, 4);
        if (bytes) {
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
            stbi_image_free(bytes);
            loaded = true;
        }
        if (!bytes) {
        std::cout << "Failed to load image: test/image.png" << std::endl;
        }
    }

    for (int i = 0; i < canvasNames.size(); i++) {
        ImGui::Begin((std::string("Canvas: ") + canvasNames[i]).c_str(), nullptr);

        ImGui::BeginChild("test", ImVec2((float)width, (float)height));

        if (texture != 0) {
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImGui::Image((ImTextureID)(intptr_t)texture, ImVec2((float)width, (float)height));
        }

        ImGui::EndChild();

        ImGui::BeginChild("file", ImVec2(150, 30));
        ImGui::SmallButton("File");
        ImGui::EndChild();

        ImGui::BeginChild("color", ImVec2(300, 300));
        ImGui::ColorPicker4("Color", myColor);
        ImGui::EndChild();

        ImGui::End();
    }
}
