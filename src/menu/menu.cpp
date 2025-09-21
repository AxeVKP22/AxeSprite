#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#endif

#include "menu.hpp"

bool isFileSubMenuOpen = false;
bool isNewPressed = false;
bool isOpenPressed = false;
bool isOpenSelected;
bool isNewSelected;

char pathToFile[MAX_PATH_LENGHT];
char newName[32];

int newWidth;
int newHeight;

std::vector<std::string> canvasNames;

//-------------------------------
// main menu render 
//-------------------------------
void imGuiRenderMenuWindow(const char* windowName) {                                                         
    ImGui::SetNextWindowSize(ImVec2(315, 150), ImGuiCond_Once);
    ImGui::Begin(windowName, nullptr);

    if (ImGui::SmallButton("File")) {
        isFileSubMenuOpen = !isFileSubMenuOpen;
    }

    ImGui::Text("\n\nWelcome to AxeSprite!\nit`s a lightweight pixel art tool\n\n\nrepo: https://github.com/AxeVKP22/AxeSprite");

    if (isFileSubMenuOpen) {
        imGuiRenderFileSubMenu();
    }

    if (isNewPressed) {
        imGuiRenderNewSubMenu();
    }

    if (isOpenPressed) {
        returnOpenCode result = imGuiRenderOpenSubMenu();
        if (result.success) {
            bool exists = false;
            for (int i = 0; i < canvasNames.size(); i++) {
                if (canvasNames[i] == result.filename) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                canvasNames.push_back(std::string(result.filename));
            }
            isOpenPressed = false;
        }
        else if (!result.success) {
            isOpenPressed = false;
        }
    }
    ImGui::End();
}

//-------------------------------
// render sub menu
//-------------------------------
void imGuiRenderFileSubMenu() {
    ImGui::SetNextWindowSize(ImVec2(120, 90), ImGuiCond_Once);
    ImGui::Begin("File submenu", nullptr, ImGuiWindowFlags_NoResize);
    if (ImGui::SmallButton("New file")) {
        isNewPressed = !isNewPressed;
    }

    if (ImGui::SmallButton("Open file")) {
        isOpenPressed = !isOpenPressed;
    }
    
    if (ImGui::SmallButton("Close")) {
        isFileSubMenuOpen = !isFileSubMenuOpen;
    }
    ImGui::End();
}

//-------------------------------
// create a new fileImGui::SetWindowSize(ImGui::GetWindowContentRegionMax());
//-------------------------------
void imGuiRenderNewSubMenu() {
    ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Always);
    ImGui::Begin("New", nullptr);
    ImGui::InputInt("width", &newWidth);
    ImGui::InputInt("height", &newHeight);
    ImGui::InputText("Name", newName, 32);

    if (ImGui::SmallButton("Create")) {
        if (newWidth > 0 && newHeight > 0 && strlen(newName) > 0) {
            bool exists = false;
            for (int i = 0; i < canvasNames.size(); i++) {
                if (canvasNames[i] == newName) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                canvasNames.push_back(std::string(newName));
                isNewPressed = false;
                isNewSelected = true;
            }
        }
    }   

    if (ImGui::SmallButton("Close")) {
        isNewPressed = !isNewPressed;
    }
    ImGui::End();
}

//-------------------------------
// get path to file
//-------------------------------
returnOpenCode imGuiRenderOpenSubMenu() {
#ifdef _WIN32
    WCHAR filename[MAX_PATH_LENGHT] = L"";

    OPENFILENAMEW ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH_LENGHT;
    ofn.lpstrFilter = L"All Files\0*.*\0\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        wcstombs(pathToFile, filename, MAX_PATH_LENGHT);

        const char* lastSlash = strrchr(pathToFile, '\\');
        if (!lastSlash) lastSlash = strrchr(pathToFile, '/');

        std::string fileNameOnly = lastSlash ? std::string(lastSlash + 1) : std::string(pathToFile);

        isOpenSelected = true;

        return {true, fileNameOnly};
    }
#endif
    return {false, ""};
}