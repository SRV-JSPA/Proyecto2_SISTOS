#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <memory>
#include <vector>
#include <map>
#include "GUI/SimulationPanel.h"
#include "Scheduler/BaseScheduler.h"
#include "Core/Process.h"

enum {
    ID_AlgorithmChoice = wxID_HIGHEST + 100,
    ID_QuantumSpin,
    ID_LoadProcessesButton
};

class SchedulerPanel : public SimulationPanel {
private:
    wxChoice* m_algorithmChoice;
    wxSpinCtrl* m_quantumSpin;
    wxStaticText* m_quantumLabel;
    wxButton* m_loadProcessesButton;
    wxTextCtrl* m_processesText;
    wxTextCtrl* m_metricsText;
    
    std::shared_ptr<BaseScheduler> m_scheduler;
    std::vector<std::shared_ptr<Process>> m_processes;
    
    void CreateScheduler();
    void UpdateMetrics();
    
    void OnAlgorithmSelected(wxCommandEvent& event);
    void OnLoadProcesses(wxCommandEvent& event);
    void OnStartSimulation(wxCommandEvent& event) override;
    void OnStopSimulation(wxCommandEvent& event) override;
    void OnResetSimulation(wxCommandEvent& event) override;
    void OnTimer(wxTimerEvent& event) override;
    
public:
    SchedulerPanel(wxWindow* parent);
    
    wxString GetResultsText() const override;
    
    wxDECLARE_EVENT_TABLE();
};