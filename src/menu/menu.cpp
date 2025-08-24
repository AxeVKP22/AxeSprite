#include "menu.hpp"

bool isFileSubMenuOpen = false;
bool isNewPressed = false;
bool isOpenPressed = false;

char pathToFile[MAX_PATH];

int newWidth;
int newheight;

void imGuiRenderMenuWindow(GLFWwindow* window, const char* windowName) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();                                                          
    ImGui::Begin(windowName, nullptr);


    if (ImGui::SmallButton("File")) {
        isFileSubMenuOpen = !isFileSubMenuOpen;
    }


    if (isFileSubMenuOpen) {
        imGuiRenderFileSubMenu();
    }

    if (isNewPressed) {
        imGuiRenderNewSubMenu();
    }

    if (isOpenPressed) {
        if (imGuiRenderOpenSubMenu() == -1) {
            isOpenPressed = !isOpenPressed;
        }
    }

    ImGui::End();
    ImGui::Render();
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}

void imGuiRenderFileSubMenu() {
    ImGui::SetNextWindowSize(ImVec2(120, 90), ImGuiCond_Always);
    ImGui::Begin("File submenu", nullptr, ImGuiWindowFlags_NoResize);
    if (ImGui::SmallButton("New")) {
        isNewPressed = !isNewPressed;
    }

    if (ImGui::SmallButton("Open")) {
        isOpenPressed = !isOpenPressed;
    }
    
    if (ImGui::SmallButton("Close")) {
        isFileSubMenuOpen = !isFileSubMenuOpen;
    }
    ImGui::End();
}

void imGuiRenderNewSubMenu() {
    ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Always);
    ImGui::Begin("New", nullptr);
    ImGui::InputInt("width", &newWidth);
    ImGui::InputInt("height", &newheight);
    if (ImGui::SmallButton("Create")) {
        
    }

    if (ImGui::SmallButton("Close")) {
        isNewPressed = !isNewPressed;
    }
    ImGui::End();
}

int imGuiRenderOpenSubMenu() {
    WCHAR filename[MAX_PATH] = L"";

    OPENFILENAMEW ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"All Files\0*.*\0\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        char buf[MAX_PATH];
        wcstombs(pathToFile, filename, MAX_PATH);
        wcstombs(buf, filename, MAX_PATH);
        return 0;
    }
    else {
        return -1;
    }
}