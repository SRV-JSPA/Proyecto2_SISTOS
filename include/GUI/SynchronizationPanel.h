#pragma once
#include <wx/wx.h>
#include <memory>
#include <vector>
#include <map>
#include "GUI/SimulationPanel.h"
#include "Synchronization/BaseSynchronization.h"
#include "Core/Process.h"
#include "Core/Resource.h"
#include "Core/Action.h"

enum {
    ID_MechanismChoice = wxID_HIGHEST + 200,
    ID_LoadProcessesButton2,
    ID_LoadResourcesButton,
    ID_LoadActionsButton
};

class SynchronizationPanel : public SimulationPanel {
private:
    wxChoice* m_mechanismChoice;
    wxButton* m_loadProcessesButton;
    wxButton* m_loadResourcesButton;
    wxButton* m_loadActionsButton;
    wxTextCtrl* m_processesText;
    wxTextCtrl* m_resourcesText;
    wxTextCtrl* m_actionsText;
    
    std::shared_ptr<BaseSynchronization> m_syncMechanism;
    std::vector<std::shared_ptr<Process>> m_processes;
    std::map<std::string, std::shared_ptr<Resource>> m_resources;
    std::vector<std::shared_ptr<Action>> m_actions;
    
    void CreateSynchronizationMechanism();
    
    void OnMechanismSelected(wxCommandEvent& event);
    void OnLoadProcesses(wxCommandEvent& event);
    void OnLoadResources(wxCommandEvent& event);
    void OnLoadActions(wxCommandEvent& event);
    void OnStartSimulation(wxCommandEvent& event) override;
    void OnStopSimulation(wxCommandEvent& event) override;
    void OnResetSimulation(wxCommandEvent& event) override;
    void OnTimer(wxTimerEvent& event) override;
    
public:
    SynchronizationPanel(wxWindow* parent);
    
    wxString GetResultsText() const override;
    
    wxDECLARE_EVENT_TABLE();
};