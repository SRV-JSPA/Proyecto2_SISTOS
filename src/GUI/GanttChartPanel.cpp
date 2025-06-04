#include "GUI/GanttChartPanel.h"
#include "Utils/ColorManager.h"

wxBEGIN_EVENT_TABLE(GanttChartPanel, wxPanel)
    EVT_PAINT(GanttChartPanel::OnPaint)
    EVT_SIZE(GanttChartPanel::OnSize)
    EVT_SCROLLWIN(GanttChartPanel::OnScroll) 
wxEND_EVENT_TABLE()

GanttChartPanel::GanttChartPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
              wxFULL_REPAINT_ON_RESIZE | wxHSCROLL),
      m_currentCycle(0), m_maxCycles(20), m_blockWidth(80), m_blockHeight(50), m_scrollPos(0) {
    
    SetBackgroundColour(*wxWHITE);
    
    SetScrollbar(wxHORIZONTAL, 0, 100, 100);
}

void GanttChartPanel::AddProcessBlock(const std::string& pid, int startCycle, int duration) {
    m_blocks.emplace_back(pid, startCycle, duration);
    
    if (startCycle + duration > m_maxCycles) {
        m_maxCycles = startCycle + duration;
        
        int pageSize = GetClientSize().GetWidth() / m_blockWidth;
        SetScrollbar(wxHORIZONTAL, m_scrollPos, pageSize, m_maxCycles + 5);
    }
    
    Refresh();
}

void GanttChartPanel::UpdateCycle(int cycle) {
    m_currentCycle = cycle;
    
    int pageSize = GetClientSize().GetWidth() / m_blockWidth;
    
    if (m_currentCycle >= m_scrollPos + pageSize) {
        m_scrollPos = m_currentCycle - pageSize + 5;
        SetScrollPos(wxHORIZONTAL, m_scrollPos);
    }
    
    Refresh();
}

void GanttChartPanel::Clear() {
    m_blocks.clear();
    m_syncBlocks.clear();  
    m_currentCycle = 0;
    m_maxCycles = 20;
    m_scrollPos = 0;
    
    SetScrollPos(wxHORIZONTAL, 0);
    
    Refresh();
}

void GanttChartPanel::OnPaint(wxPaintEvent& WXUNUSED(event)) {
    wxPaintDC dc(this);
    
    wxSize size = GetClientSize();
    
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    
    dc.SetDeviceOrigin(-m_scrollPos * m_blockWidth, 0);
    
    DrawGrid(dc);
    if (!m_syncBlocks.empty()) {
        DrawSyncBlocks(dc);  
    } else {
        DrawBlocks(dc);  
    }

    DrawCycleMarkers(dc);
}

void GanttChartPanel::DrawGrid(wxDC& dc) {
    wxSize size = GetClientSize();
    int width = size.GetWidth() + m_maxCycles * m_blockWidth;
    int height = size.GetHeight();
    
    dc.SetPen(wxPen(wxColour(200, 200, 200), 1, wxPENSTYLE_SOLID));
    
    for (int y = 0; y <= height; y += m_blockHeight) {
        dc.DrawLine(0, y, width, y);
    }
    
    for (int x = 0; x <= m_maxCycles; x++) {
        dc.DrawLine(x * m_blockWidth, 0, x * m_blockWidth, height);
    }
}

void GanttChartPanel::DrawBlocks(wxDC& dc) {
    for (const auto& block : m_blocks) {
        wxColour color = ColorManager::GetColor(block.pid);
        
        int x = block.startCycle * m_blockWidth;
        int width = block.duration * m_blockWidth;
        
        dc.SetBrush(wxBrush(color));
        dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));
        dc.DrawRectangle(x, m_blockHeight, width, m_blockHeight);
        
        dc.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        dc.SetTextForeground(wxColour(0, 0, 0));
        
        wxString text = wxString(block.pid);
        wxSize textSize = dc.GetTextExtent(text);
        int textX = x + (width - textSize.GetWidth()) / 2;
        int textY = m_blockHeight + (m_blockHeight - textSize.GetHeight()) / 2;
        
        if (textSize.GetWidth() <= width - 4) {
            dc.DrawText(text, textX, textY);
        }
    }
}

void GanttChartPanel::DrawCycleMarkers(wxDC& dc) {
    dc.SetTextForeground(wxColour(0, 0, 0));
    dc.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    
    for (int i = 0; i <= m_maxCycles; i++) {
        wxString text = wxString::Format("%d", i);
        wxSize textSize = dc.GetTextExtent(text);
        
        int textX = i * m_blockWidth + (m_blockWidth - textSize.GetWidth()) / 2;
        int textY = (m_blockHeight - textSize.GetHeight()) / 2;
        
        dc.DrawText(text, textX, textY);
    }
    
    if (m_currentCycle <= m_maxCycles) {
        int x = m_currentCycle * m_blockWidth;
        wxSize size = GetClientSize();
        
        dc.SetPen(wxPen(wxColour(255, 0, 0), 2, wxPENSTYLE_SOLID));
        dc.DrawLine(x, 0, x, size.GetHeight());
    }
}

void GanttChartPanel::OnScroll(wxScrollWinEvent& event) {
    m_scrollPos = event.GetPosition();
    Refresh();
}

void GanttChartPanel::OnSize(wxSizeEvent& event) {
    int pageSize = GetClientSize().GetWidth() / m_blockWidth;
    SetScrollbar(wxHORIZONTAL, m_scrollPos, pageSize, m_maxCycles + 5);
    
    event.Skip();
}
void GanttChartPanel::DrawSyncBlocks(wxDC& dc) {
    int rowHeight = m_blockHeight;
    std::map<std::string, int> processRows;  
    int currentRow = 1;
    
    for (const auto& block : m_syncBlocks) {
        std::string processId = block.pid;
        
        if (processRows.find(processId) == processRows.end()) {
            processRows[processId] = currentRow++;
        }
        
        int row = processRows[processId];
        int x = block.startCycle * m_blockWidth;
        int y = row * rowHeight;
        int width = block.duration * m_blockWidth;
        
        wxColour color;
        if (block.isWaiting) {
            color = wxColour(255, 200, 200);  
        } else {
            color = ColorManager::GetColor(block.pid);  
        }
        
        dc.SetBrush(wxBrush(color));
        dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));
        dc.DrawRectangle(x, y, width, rowHeight);
        
        dc.SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        dc.SetTextForeground(wxColour(0, 0, 0));
        
        wxString text = wxString::Format("%s_%s%s", 
                                       block.pid, 
                                       block.resourceName,
                                       block.isWaiting ? "_W" : "");  
        
        wxSize textSize = dc.GetTextExtent(text);
        int textX = x + (width - textSize.GetWidth()) / 2;
        int textY = y + (rowHeight - textSize.GetHeight()) / 2;
        
        if (textSize.GetWidth() <= width - 4) {
            dc.DrawText(text, textX, textY);
        }
    }
}


void GanttChartPanel::AddSyncBlock(const std::string& pid, const std::string& resourceName, 
                                   int startCycle, int duration, bool isWaiting) {
    m_syncBlocks.emplace_back(pid, resourceName, startCycle, duration, isWaiting);
    
    if (startCycle + duration > m_maxCycles) {
        m_maxCycles = startCycle + duration;
        int pageSize = GetClientSize().GetWidth() / m_blockWidth;
        SetScrollbar(wxHORIZONTAL, m_scrollPos, pageSize, m_maxCycles + 5);
    }
    
    Refresh();
}
