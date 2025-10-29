#include "canvas.hpp"

float myColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float transparent[4] = {0.0f, 0.0f, 0.0f, 0.0f};
std::vector<std::array<float, 4>> recentColors;

ImGuiWindowFlags windowFlags = 0;
bool blockMove = false;

bool initialized = false;
Texture2D texture;
Image image;
float zoom = 1.0f;
ImVec2 resolution;

bool NisFileSubMenuOpen = false;

int drawingMode = 0; // 0 - point drawing, 1 - line drawing
const char* drawingModeList[] = {"Point drawing", "Line drawing", "Circle drawing"};
ImVec2 startPos;
ImVec2 finishPos;

//-------------------------------
// RenderCanvasWindow "Rustafied.com - US Long, T17"
//-------------------------------
void imGuiRenderCanvasWindow(const char* windowName) {
    for (int i = 0; i < canvasNames.size(); i++) {

        
        if (NisFileSubMenuOpen) { NimGuiRenderFileSubMenu(); }

        //-------------------------------
        // create/load texture
        //-------------------------------
        if (!initialized) { createImage(); }

        ImGui::Begin((std::string("Canvas: ") + canvasNames[i]).c_str(), 0, windowFlags);

        ImGui::BeginChild("file", ImVec2(150, 30));
            if (ImGui::SmallButton("File")) {
                NisFileSubMenuOpen = !NisFileSubMenuOpen;
            }
        ImGui::EndChild();

        //-------------------------------
        // color picker, recent colors and choosing drawing mode
        //-------------------------------
        drawColorUI();

        //-------------------------------
        // zoom
        //-------------------------------
        float mouse = ImGui::GetIO().MouseWheel;
        if (!ImGui::IsWindowHovered() && mouse != 0.0f) {
            zoom += (mouse + 0.1f);
            if (zoom < 0.1f) {              
                zoom = 0.1f;
            }
        }
        ImGui::SameLine();

        //-------------------------------
        // canvas
        //-------------------------------
        canvasWindow();
        
        ImGui::End();
        //-------------------------------
        // set ImGuiWindowFlags_NoMove if mouse in "Pixel Window" area
        //-------------------------------
        if (blockMove) {
            windowFlags |= ImGuiWindowFlags_NoMove;
        }
        else {
            windowFlags &= ~ImGuiWindowFlags_NoMove;
        }
    }
}

//-------------------------------
// render sub menu
//-------------------------------
void NimGuiRenderFileSubMenu() {
    ImGui::SetNextWindowSize(ImVec2(120, 90), ImGuiCond_Once);
    ImGui::Begin("Canvas file submenu", nullptr);

    if (ImGui::SmallButton("Save")) {
        std::string path = imGuiSaveAs();
        ExportImage(image, path.c_str());
    }

    if (ImGui::SmallButton("Close")) {
        NisFileSubMenuOpen = !NisFileSubMenuOpen;
    }

    if (ImGui::SmallButton("Exit")) {
        CloseWindow();
    }
    ImGui::End();
}

//-------------------------------
// an func for create/load image and texture :)
//-------------------------------
void createImage() {
    if (isOpenSelected) {
        if (texture.id > 0) {
            UnloadTexture(texture); 
            UnloadImage(image);
        }
        image = LoadImage(pathToFile);
        texture = LoadTexture(pathToFile);
    } 

    else if (isNewSelected) {
        if (texture.id > 0) {
            UnloadTexture(texture);
            UnloadImage(image);
        }
        switch (selectedItem) {
            case 0:
                image = GenImageColor(newWidth, newHeight, WHITE);
                texture = LoadTextureFromImage(image);
                break;
            case 1:
                image = GenImageColor(newWidth, newHeight, BLACK);
                texture = LoadTextureFromImage(image);
                break;
            case 2:
                image = GenImageColor(newWidth, newHeight, BLANK);
                texture = LoadTextureFromImage(image);
                break;
        }
    }
    initialized = true;
}

//-------------------------------
// color picker, recent colors and choosing drawing mode function 
//-------------------------------
void drawColorUI() {
    ImGui::BeginChild("color", ImVec2(310, 340));
        ImGui::ColorPicker4("Color", myColor);
        for (int i = 0; i < recentColors.size(); i++) {
            ImVec4 c = ImVec4(
                recentColors[i][0],
                recentColors[i][1],
                recentColors[i][2],
                recentColors[i][3]
            );

            std::string id = "##col" + std::to_string(i);
            if (ImGui::ColorButton(id.c_str(), c, 0, ImVec2(16, 16))) {
                myColor[0] = recentColors[i][0];
                myColor[1] = recentColors[i][1];
                myColor[2] = recentColors[i][2];
                myColor[3] = recentColors[i][3];
            }

            if ((i + 1) % 6 != 0) {
                ImGui::SameLine();
            }

        }  

        ImGui::NewLine();

        if (ImGui::BeginCombo("Drawing mode", drawingModeList[drawingMode])) {
            for (int i = 0; i < IM_ARRAYSIZE(drawingModeList); i++) {
                bool isSelected = (drawingMode == i);
                if (ImGui::Selectable(drawingModeList[i], isSelected)) {
                    drawingMode = i;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

    ImGui::EndChild(); 
}

//-------------------------------
// render/draw/erase func
//-------------------------------
void canvasWindow() {
    blockMove = false;
    ImGui::BeginChild("Pixel Window", ImVec2(0, 0), true);
        if (ImGui::IsWindowHovered()) {
            blockMove = true;
        }


        //-------------------------------
        // background
        //-------------------------------
        resolution = ImGui::GetContentRegionAvail();
        ImVec2 textureSize = ImVec2(texture.width * zoom, texture.height * zoom);
        ImVec2 windowPos = ImGui::GetCursorScreenPos();

        float cellSize = 8.0f;

        ImVec2 texturePos = ImVec2(
            windowPos.x + std::max((resolution.x - textureSize.x) * 0.5f, 0.0f),
            windowPos.y + std::max((resolution.y - textureSize.y) * 0.5f, 0.0f)
        );

        Image checker = GenImageColor(2, 2, BLACK);

        Color* pixels = (Color*)checker.data;

        pixels[0] = (Color){255, 255, 255, 255};
        pixels[1] = (Color){200, 200, 200, 255};
        pixels[2] = (Color){200, 200, 200, 255};
        pixels[3] = (Color){255, 255, 255, 255};

        Texture2D checkerTex = LoadTextureFromImage(checker);
        UnloadImage(checker);

        float scale = cellSize * zoom;

        Rectangle src = { 0.0f, 0.0f, (float)checkerTex.width, (float)checkerTex.height };
        Rectangle dst = { texturePos.x, texturePos.y, textureSize.x, textureSize.y };

        SetTextureWrap(checkerTex, TEXTURE_WRAP_REPEAT);

        src.width  *= textureSize.x / scale;
        src.height *= textureSize.y / scale;

        ImGui::SetCursorScreenPos(texturePos);
        ImGui::Image((void*)(intptr_t)texture.id, textureSize);
        DrawTexturePro(checkerTex, src, dst, {0, 0}, 0, WHITE);
        drawPixelGrid(texturePos, textureSize, texture, zoom);

        //-------------------------------
        // draw/erase
        //-------------------------------
        if (ImGui::IsWindowHovered()) {
            switch (drawingMode) {
                //-------------------------------
                // point drawing
                //-------------------------------
                case 0:
                    if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                        std::array<float,4> temColor = { myColor[0], myColor[1], myColor[2], myColor[3] };
                        
                        const int maxColors = 18;
                        bool exists = false;

                        for (const auto& c : recentColors) {
                            if (c == temColor) {
                                exists = true;
                                break;
                            }
                        }

                        if (!exists) {
                            recentColors.push_back(temColor);

                            if (recentColors.size() > maxColors) {
                                recentColors.erase(recentColors.begin());
                            }
                        } 

                        ImVec2 mousePos = ImGui::GetMousePos();
                        ImVec2 windowPos = ImGui::GetItemRectMin();
                        float localX = (mousePos.x - windowPos.x) / zoom;
                        float localY = (mousePos.y - windowPos.y) / zoom;

                        int pixelX = (int)localX;
                        int pixelY = (int)localY;
                            
                        drawPixel(myColor, pixelX, pixelY);

                    }
                break;

                //-------------------------------
                // line drawing
                //-------------------------------
                case 1:
                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                        ImVec2 mousePos = ImGui::GetMousePos();
                        ImVec2 windowPos = ImGui::GetItemRectMin();
                        float localX = (mousePos.x - windowPos.x) / zoom;
                        float localY = (mousePos.y - windowPos.y) / zoom;

                        int pixelX = (int)localX;
                        int pixelY = (int)localY;

                        startPos.x = pixelX;
                        startPos.y = pixelY;

                    }

                    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                        std::array<float,4> temColor = { myColor[0], myColor[1], myColor[2], myColor[3] };
                        
                        const int maxColors = 18;
                        bool exists = false;

                        for (const auto& c : recentColors) {
                            if (c == temColor) {
                                exists = true;
                                break;
                            }
                        }

                        if (!exists) {
                            recentColors.push_back(temColor);

                            if (recentColors.size() > maxColors) {
                                recentColors.erase(recentColors.begin());
                            }
                        } 

                        ImVec2 mousePos = ImGui::GetMousePos();
                        ImVec2 windowPos = ImGui::GetItemRectMin();
                        float localX = (mousePos.x - windowPos.x) / zoom;
                        float localY = (mousePos.y - windowPos.y) / zoom;

                        int pixelX = (int)localX;
                        int pixelY = (int)localY;

                        finishPos.x = pixelX;
                        finishPos.y = localY;

                        drawLine(startPos.x, startPos.y, finishPos.x, finishPos.y);
                    }
                break;

                //-------------------------------
                // circle drawing
                //-------------------------------
                case 2:
                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                        ImVec2 mousePos = ImGui::GetMousePos();
                        ImVec2 windowPos = ImGui::GetItemRectMin();
                        float localX = (mousePos.x - windowPos.x) / zoom;
                        float localY = (mousePos.y - windowPos.y) / zoom;

                        int pixelX = (int)localX;
                        int pixelY = (int)localY;

                        startPos.x = pixelX;
                        startPos.y = pixelY;

                    }

                    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                        std::array<float,4> temColor = { myColor[0], myColor[1], myColor[2], myColor[3] };
                        
                        const int maxColors = 18;
                        bool exists = false;

                        for (const auto& c : recentColors) {
                            if (c == temColor) {
                                exists = true;
                                break;
                            }
                        }

                        if (!exists) {
                            recentColors.push_back(temColor);

                            if (recentColors.size() > maxColors) {
                                recentColors.erase(recentColors.begin());
                            }
                        } 

                        ImVec2 mousePos = ImGui::GetMousePos();
                        ImVec2 windowPos = ImGui::GetItemRectMin();
                        float localX = (mousePos.x - windowPos.x) / zoom;
                        float localY = (mousePos.y - windowPos.y) / zoom;

                        int pixelX = (int)localX;
                        int pixelY = (int)localY;

                        finishPos.x = pixelX;
                        finishPos.y = localY;

                        ImVec2 center = ImVec2((std::round(startPos.x + finishPos.x) / 2), (std::round(startPos.y +  finishPos.y) / 2));
                        int dx = finishPos.x - startPos.x;
                        int dy = finishPos.y - startPos.y;
                        int radius = std::min(dx, dy) / 2;

                       drawCircle(center.x, center.y, radius);
                    }
                break;
            }
                
            //-------------------------------
            //erase
            //-------------------------------
            if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
                ImVec2 mousePos = ImGui::GetMousePos();
                ImVec2 windowPos = ImGui::GetItemRectMin();
                float localX = (mousePos.x - windowPos.x) / zoom;
                float localY = (mousePos.y - windowPos.y) / zoom;

                int pixelX = (int)localX;
                int pixelY = (int)localY;

                drawPixel(transparent, pixelX, pixelY);
            }
        }

    ImGui::EndChild();
}

//-------------------------------
// draw line function
//-------------------------------
void drawLine(int x0, int y0, int x1, int y1) {
    int dx = std::abs(x1 - x0);
    int dy = -std::abs(y1 - y0);

    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;

    int error = dx+dy;

    while (true) {
        drawPixel(myColor, x0, y0);

        if (x0 == x1 and y0 == y1) {
            break;
        }

        int error2 = 2 * error;

        if (error2 >= dy) {
            error += dy;
            x0 += sx;
        }

        if (error2 <= dx) {
            error += dx;
            y0 += sy;
        }
    }
}

//-------------------------------
// draw circle function
//-------------------------------
void drawCircle(int xm, int ym, int r)
{
   int x = -r, y = 0, err = 2-2*r;
   do {
      drawPixel(myColor, xm-x, ym+y);
      drawPixel(myColor, xm-y, ym-x);
      drawPixel(myColor, xm+x, ym-y);
      drawPixel(myColor, xm+y, ym+x);
      r = err;
      if (r <= y) err += ++y*2+1;
      if (r > x || err > y) err += ++x*2+1;
   } while (x < 0);
}

//-------------------------------
// draw pixel function
//-------------------------------
void drawPixel(const float* color, int x, int y) {
    if (x >= 0 && y < image.width && y >= 0 && y < image.height) {
        Color c = {
        (unsigned char)(color[0] * 255),
        (unsigned char)(color[1] * 255),
        (unsigned char)(color[2] * 255),
        (unsigned char)(color[3] * 255)
    };
            
    ImageDrawPixel(&image, x, y, c);
    UpdateTexture(texture, image.data);
    }
}

//-------------------------------
// pixel grid function
//-------------------------------
void drawPixelGrid(ImVec2 pos, ImVec2 size, Texture2D tex, float zm) {
    if (zm < 7.0f) return;

    float cellsize = zm;
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImU32 lineColor = IM_COL32(0, 0, 0, 60);

    for (int x = 0; x <= tex.width; x++) {
        float posX = pos.x + x * cellsize;
        drawList->AddLine(ImVec2(posX, pos.y), ImVec2(posX, pos.y + size.y), lineColor);
    }

    for (int y = 0; y <= tex.height; y++) {
        float posY = pos.y + y * cellsize;
        drawList->AddLine(ImVec2(pos.x, posY), ImVec2(pos.x + size.x, posY), lineColor);
    }
}