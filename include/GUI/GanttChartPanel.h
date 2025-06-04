#pragma once
#include <wx/wx.h>
#include <vector>
#include <map>
#include <string>

struct ProcessBlock {
    std::string pid;
    int startCycle;
    int duration;
    
    ProcessBlock(const std::string& pid, int startCycle, int duration)
        : pid(pid), startCycle(startCycle), duration(duration) {}
};

struct SyncBlock {
    std::string pid;
    std::string resourceName;
    int startCycle;
    int duration;
    bool isWaiting;  
    
    SyncBlock(const std::string& pid, const std::string& resourceName, 
              int startCycle, int duration, bool isWaiting)
        : pid(pid), resourceName(resourceName), startCycle(startCycle), 
          duration(duration), isWaiting(isWaiting) {}
};

class GanttChartPanel : public wxPanel {
private:
    std::vector<ProcessBlock> m_blocks;        
    std::vector<SyncBlock> m_syncBlocks;       
    int m_currentCycle;
    int m_maxCycles;
    double m_blockWidth;
    double m_blockHeight;
    int m_scrollPos;
    std::map<std::string, wxColour> m_processColors;
    
    void DrawGrid(wxDC& dc);
    void DrawBlocks(wxDC& dc);
    void DrawCycleMarkers(wxDC& dc);
    void DrawSyncBlocks(wxDC& dc);  
    
public:
    GanttChartPanel(wxWindow* parent);
    
    void AddProcessBlock(const std::string& pid, int startCycle, int duration);
    void AddSyncBlock(const std::string& pid, const std::string& resourceName, 
                      int startCycle, int duration, bool isWaiting);
    void UpdateCycle(int cycle);
    void Clear();
    
    void OnPaint(wxPaintEvent& event);
    void OnScroll(wxScrollWinEvent& event);  
    void OnSize(wxSizeEvent& event);
    
    wxDECLARE_EVENT_TABLE();
};