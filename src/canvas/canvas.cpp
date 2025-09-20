#include "canvas.hpp"

float myColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float transparent[4] = {0.0f, 0.0f, 0.0f, 0.0f};

ImGuiWindowFlags windowFlags = 0;

std::vector<Color> pixels;
std::vector<std::array<float, 4>> recentColors;

bool initialized = false;
Texture2D texture;
Image image;
float zoom = 1.0f;
ImVec2 resolution;

//-------------------------------
// RenderCanvasWindow "Rustafied.com - US Long, T17"
//-------------------------------
void imGuiRenderCanvasWindow(const char* windowName) {
    for (int i = 0; i < canvasNames.size(); i++) {

        //-------------------------------
        // create/load texture
        //-------------------------------
        if (!initialized) {
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
                image = GenImageColor(newWidth, newHeight, WHITE);
                texture = LoadTextureFromImage(image);
            }
            initialized = true;
        }

        ImGui::Begin((std::string("Canvas: ") + canvasNames[i]).c_str(), 0, windowFlags);

        ImGui::BeginChild("file", ImVec2(150, 30));
            /*if (ImGui::SmallButton("File")) {
                imGuiSaveAs(image);
                UnloadImage(image);
            }*/
        ImGui::EndChild();

        //-------------------------------
        // color picker and recent colors
        //-------------------------------
        ImGui::BeginChild("color", ImVec2(300, 340));
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
        ImGui::EndChild(); 

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
        bool blockMove = false;

        ImGui::BeginChild("Pixel Window", ImVec2(0, 0), true);
            if (ImGui::IsWindowHovered()) {
                blockMove = true;
            }

            ImVec2 resolution = ImGui::GetContentRegionAvail();
            ImVec2 textureSize = ImVec2(texture.width * zoom, texture.height * zoom);
            ImVec2 texturePos;

            texturePos.x = std::max((resolution.x - textureSize.x) * 0.5f, 0.0f);
            texturePos.y = std::max((resolution.y - textureSize.y) * 0.5f, 0.0f);

            ImGui::SetCursorPos(texturePos);
            ImGui::Image((void*)(intptr_t)texture.id, textureSize);

            //-------------------------------
            // draw/erase
            //-------------------------------
            if (ImGui::IsWindowHovered()) {

                //-------------------------------
                // draw
                //-------------------------------
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

                    if (pixelX >= 0 && pixelY < image.width && pixelY >= 0 && pixelY < image.height) {
                        Color c = {
                            (unsigned char)(myColor[0] * 255),
                            (unsigned char)(myColor[1] * 255),
                            (unsigned char)(myColor[2] * 255),
                            (unsigned char)(myColor[3] * 255)
                        };
            
                        ImageDrawPixel(&image, pixelX, pixelY, c);
                        UpdateTexture(texture, image.data);
                    }
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

                    if (pixelX >= 0 && pixelY < image.width && pixelY >= 0 && pixelY < image.height) {
                        Color c = {
                            (unsigned char)(transparent[0] * 255),
                            (unsigned char)(transparent[1] * 255),
                            (unsigned char)(transparent[2] * 255),
                            (unsigned char)(transparent[3] * 255)
                        };
            
                        ImageDrawPixel(&image, pixelX, pixelY, c);
                        UpdateTexture(texture, image.data);
                    }
                }
            }

        ImGui::EndChild();
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

/*void imGuiSaveAs(Image image) {
    #ifdef _WIN32
        char szFile[MAX_PATH];

        OPENFILENAME ofn = {};
        ofn.lStructSize = sizeof(ofn);
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = "PNG Files\0*.png\0All Files\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrTitle = "Save As...";
        ofn.Flags = OFN_OVERWRITEPROMPT;
        if (GetSaveFileName(&ofn)) {
            std::string fl = ofn.lpstrFile;
            if (fl.find('.') == std::string::npos) {
                fl += ".png";
            }
            ExportImage(image, fl.c_str());
        }
    #endif
}*/