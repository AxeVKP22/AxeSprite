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
    if (!glfwGetCurrentContext()) {
        std::cout << "No OpenGL context active! Cannot load texture yet." << std::endl;
        return; // ждем контекста
    }

    int numChannels;
    unsigned char* bytes = stbi_load("../test/boards_demo_11.png", &width, &height, &numChannels, STBI_rgb_alpha);
    if (!bytes) {
        std::cout << "FAILED TO LOAD IMAGE!" << std::endl;
        return;
    }

    std::cout << "Loaded " << width << "x" << height << std::endl;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, bytes);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "OpenGL error after glTexImage2D: " << err << std::endl;
    }

    stbi_image_free(bytes);
    loaded = true;
}


    for (int i = 0; i < canvasNames.size(); i++) {
        ImGui::Begin((std::string("Canvas: ") + canvasNames[i]).c_str(), nullptr);

        ImGui::BeginChild("test", ImVec2((float)width, (float)height));

        if (texture != 0) {
            ImGui::Image((ImTextureID)(intptr_t)texture, ImVec2((float)width, (float)height));
        }
        else {
            ImGui::Text("Texture not loaded");
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
