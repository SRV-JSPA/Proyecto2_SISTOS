#include "Utils/ColorManager.h"

std::map<std::string, wxColour> ColorManager::s_colors;
int ColorManager::s_colorIndex = 0;

wxColour ColorManager::GenerateUniqueColor() {
    static const wxColour predefinedColors[] = {
        wxColour(255, 99, 71),   
        wxColour(30, 144, 255),  
        wxColour(50, 205, 50),   
        wxColour(255, 215, 0),   
        wxColour(138, 43, 226),  
        wxColour(255, 127, 80),  
        wxColour(0, 191, 255),   
        wxColour(60, 179, 113),  
        wxColour(238, 130, 238), 
        wxColour(165, 42, 42),   
        wxColour(0, 128, 128),   
        wxColour(128, 0, 128),  
        wxColour(0, 128, 0),     
        wxColour(128, 0, 0),     
        wxColour(0, 0, 128)      
    };
    
    const int numColors = sizeof(predefinedColors) / sizeof(predefinedColors[0]);
    wxColour color = predefinedColors[s_colorIndex % numColors];
    s_colorIndex++;
    
    return color;
}

wxColour ColorManager::GetColor(const std::string& pid) {
    if (s_colors.find(pid) == s_colors.end()) {
        s_colors[pid] = GenerateUniqueColor();
    }
    
    return s_colors[pid];
}

void ColorManager::Reset() {
    s_colors.clear();
    s_colorIndex = 0;
}