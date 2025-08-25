#include "menu.hpp"

bool isFileSubMenuOpen = false;
bool isNewPressed = false;
bool isOpenPressed = false;

char pathToFile[MAX_PATH_LENGHT];

int newWidth;
int newheight;

char newName[32];
std::vector<std::string> canvasNames;

void imGuiRenderMenuWindow(GLFWwindow* window, const char* windowName) {                                                         
    ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Always);
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
    ImGui::InputText("Name", newName, 32);

    if (ImGui::SmallButton("Create")) {
    if (newWidth > 0 && newheight > 0 && strlen(newName) > 0) {
        bool exists = false;
        for (int i = 0; i < canvasNames.size(); i++) {
            if (canvasNames[i] == newName) {
                exists = true;  // нашли такое имя
                break;
            }
        }

        if (!exists) {
            canvasNames.push_back(std::string(newName));
            renderCanvasWindow = true;
            isNewPressed = false;
        }
    }
}

    if (ImGui::SmallButton("Close")) {
        isNewPressed = !isNewPressed;
    }
    ImGui::End();
}

int imGuiRenderOpenSubMenu() {
    WCHAR filename[MAX_PATH_LENGHT] = L"";

    OPENFILENAMEW ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH_LENGHT;
    ofn.lpstrFilter = L"All Files\0*.*\0\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        char buf[MAX_PATH_LENGHT];
        wcstombs(pathToFile, filename, MAX_PATH_LENGHT);
        wcstombs(buf, filename, MAX_PATH_LENGHT);
        return 0;
    }
    else {
        return -1;
    }
}