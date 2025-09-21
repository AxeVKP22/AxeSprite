#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <commdlg.h>
#endif

#include "save.hpp"

std::string imGuiSaveAs() {
    #ifdef _WIN32
        char szFile[MAX_PATH] = {0};

        OPENFILENAME ofn = {};
        ofn.lStructSize = sizeof(ofn);
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = "PNG Files\0*.png\0All Files\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrTitle = "Save...";
        ofn.Flags = OFN_OVERWRITEPROMPT;
        if (GetSaveFileName(&ofn)) {
            std::string fl = ofn.lpstrFile;
            if (fl.find('.') == std::string::npos) {
                fl += ".png";
            }
            return fl;
        }
    #endif
    return "";
}