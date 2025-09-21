#include <iostream>
#include "init/imgui.hpp"
#include "menu/menu.hpp"
#include "canvas/canvas.hpp"

Color blackGray = {20, 20, 20, 255};

int main() {
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    rlCreateWindow(800, 800, "AxeSprite");

    Image icon = LoadImage("C:\\GitHub\\AxeSprite\\tests\\test.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    rlImGuiSetup(true);
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.IniFilename = nullptr;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(blackGray);

        rlImGuiBegin();

        imGuiRenderMenuWindow("Menu");

        if (!canvasNames.empty()) {
            imGuiRenderCanvasWindow("Canvas");
        }
        
        rlImGuiEnd();
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
