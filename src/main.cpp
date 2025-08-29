#include <iostream>
#include "globals.hpp"
#include "init/imgui.hpp"
#include "menu/menu.hpp"
#include "canvas/canvas.hpp"

Color blackGray = {20, 20, 20, 255};

int main() {

    rlCreateWindow(800, 800, "AxeSprite");
    rlImGuiSetup(true);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(blackGray);

        rlImGuiBegin();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        io.IniFilename = nullptr;

        imGuiRenderMenuWindow("Menu");

        if (!canvasNames.empty()) {
            imGuiRenderCanvasWindow("Canvas");

        }
        rlImGuiEnd();
        EndDrawing();
    }
    rlImGuiShutdown();
    CloseWindow();

    return 0;
}
