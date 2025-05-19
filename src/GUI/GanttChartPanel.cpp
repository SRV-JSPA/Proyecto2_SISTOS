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
      m_currentCycle(0), m_maxCycles(20), m_blockWidth(30), m_blockHeight(30), m_scrollPos(0) {
    
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
    DrawBlocks(dc);
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
        
        dc.SetTextForeground(wxColour(0, 0, 0));
        dc.DrawText(block.pid, x + 5, m_blockHeight + 5);
    }
}

void GanttChartPanel::DrawCycleMarkers(wxDC& dc) {
    dc.SetTextForeground(wxColour(0, 0, 0));
    dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    
    for (int i = 0; i <= m_maxCycles; i++) {
        wxString text = wxString::Format("%d", i);
        dc.DrawText(text, i * m_blockWidth + 5, 5);
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