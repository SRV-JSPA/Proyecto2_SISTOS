#pragma once
#include <string>
#include <map>
#include <wx/colour.h>

class ColorManager {
private:
    static std::map<std::string, wxColour> s_colors;
    static int s_colorIndex;
    
    static wxColour GenerateUniqueColor();
    
public:
    static wxColour GetColor(const std::string& pid);
    static void Reset();
};