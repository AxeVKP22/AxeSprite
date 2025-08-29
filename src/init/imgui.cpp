#include "imgui.hpp"

void rlCreateWindow(int width, int height, const char* title) {
    InitWindow(width, height, title);
    SetTargetFPS(60);
}

void rlShutdownWindow() {
    CloseWindow();
}