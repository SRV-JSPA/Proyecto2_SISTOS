#pragma once
#include <wx/wx.h>
#include "GUI/GanttChartPanel.h"

class SimulationPanel : public wxPanel {
protected:
    wxButton* m_startButton;
    wxButton* m_stopButton;
    wxButton* m_resetButton;
    wxStaticText* m_cycleText;
    GanttChartPanel* m_ganttChart;
    wxTimer m_timer;
    bool m_isRunning;
    
    virtual void OnStartSimulation(wxCommandEvent& event) = 0;
    virtual void OnStopSimulation(wxCommandEvent& event) = 0;
    virtual void OnResetSimulation(wxCommandEvent& event) = 0;
    virtual void OnTimer(wxTimerEvent& event) = 0;
    
public:
    SimulationPanel(wxWindow* parent);
    virtual ~SimulationPanel();
    
    virtual wxString GetResultsText() const = 0;
    
    wxDECLARE_EVENT_TABLE();
};