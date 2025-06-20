#include "GUI/SynchronizationPanel.h"
#include "Synchronization/MutexLock.h"
#include "Synchronization/Semaphore.h"
#include "Utils/FileParser.h"
#include <wx/filedlg.h>
#include <sstream>

wxBEGIN_EVENT_TABLE(SynchronizationPanel, SimulationPanel)
    EVT_CHOICE(ID_MechanismChoice, SynchronizationPanel::OnMechanismSelected)
    EVT_BUTTON(ID_LoadProcessesButton2, SynchronizationPanel::OnLoadProcesses)
    EVT_BUTTON(ID_LoadResourcesButton, SynchronizationPanel::OnLoadResources)
    EVT_BUTTON(ID_LoadActionsButton, SynchronizationPanel::OnLoadActions)
wxEND_EVENT_TABLE()

SynchronizationPanel::SynchronizationPanel(wxWindow* parent)
    : SimulationPanel(parent) {
    
    wxStaticText* mechanismLabel = new wxStaticText(this, wxID_ANY, "Mecanismo:");
    
    wxArrayString choices;
    choices.Add("Mutex Locks");
    choices.Add("Semáforos");
    
    m_mechanismChoice = new wxChoice(this, ID_MechanismChoice, wxDefaultPosition, wxDefaultSize, choices);
    m_mechanismChoice->SetSelection(0);
    
    m_loadProcessesButton = new wxButton(this, ID_LoadProcessesButton2, "Cargar Procesos");
    m_loadResourcesButton = new wxButton(this, ID_LoadResourcesButton, "Cargar Recursos");
    m_loadActionsButton = new wxButton(this, ID_LoadActionsButton, "Cargar Acciones");
    
    wxStaticBoxSizer* processesSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Procesos");
    m_processesText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 
                                    wxTE_MULTILINE | wxTE_READONLY);
    processesSizer->Add(m_processesText, 1, wxEXPAND | wxALL, 5);
    
    wxStaticBoxSizer* resourcesSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Recursos");
    m_resourcesText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 
                                    wxTE_MULTILINE | wxTE_READONLY);
    resourcesSizer->Add(m_resourcesText, 1, wxEXPAND | wxALL, 5);
    
    wxStaticBoxSizer* actionsSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Acciones");
    m_actionsText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 
                                 wxTE_MULTILINE | wxTE_READONLY);
    actionsSizer->Add(m_actionsText, 1, wxEXPAND | wxALL, 5);
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* configSizer = new wxBoxSizer(wxHORIZONTAL);
    configSizer->Add(mechanismLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    configSizer->Add(m_mechanismChoice, 0, wxALL, 5);
    configSizer->Add(m_loadProcessesButton, 0, wxALL, 5);
    configSizer->Add(m_loadResourcesButton, 0, wxALL, 5);
    configSizer->Add(m_loadActionsButton, 0, wxALL, 5);
    
    mainSizer->Add(configSizer, 0, wxEXPAND);
    mainSizer->Add(m_startButton, 0, wxALL, 5);
    mainSizer->Add(m_stopButton, 0, wxALL, 5);
    mainSizer->Add(m_resetButton, 0, wxALL, 5);
    mainSizer->Add(m_cycleText, 0, wxALL, 5);
    
    wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);
    contentSizer->Add(processesSizer, 1, wxEXPAND | wxALL, 5);
    contentSizer->Add(resourcesSizer, 1, wxEXPAND | wxALL, 5);
    contentSizer->Add(actionsSizer, 1, wxEXPAND | wxALL, 5);
    
    mainSizer->Add(contentSizer, 0, wxEXPAND);
    mainSizer->Add(m_ganttChart, 1, wxEXPAND | wxALL, 5);
    
    SetSizer(mainSizer);
    
    CreateSynchronizationMechanism();
}

void SynchronizationPanel::CreateSynchronizationMechanism() {
    int selection = m_mechanismChoice->GetSelection();
    
    switch (selection) {
        case 0:
            m_syncMechanism = std::make_shared<MutexLock>();
            break;
        case 1: 
            m_syncMechanism = std::make_shared<Semaphore>();
            break;
        default:
            m_syncMechanism = std::make_shared<MutexLock>();
            break;
    }
    
    if (!m_processes.empty()) {
        m_syncMechanism->LoadProcesses(m_processes);
    }
    
    if (!m_resources.empty()) {
        m_syncMechanism->LoadResources(m_resources);
    }
    
    if (!m_actions.empty()) {
        m_syncMechanism->LoadActions(m_actions);
    }
    
    if (!m_processes.empty() && !m_resources.empty() && !m_actions.empty()) {
        m_syncMechanism->Initialize();
    }
}

void SynchronizationPanel::OnMechanismSelected(wxCommandEvent& event) {
    CreateSynchronizationMechanism();
}

void SynchronizationPanel::OnLoadProcesses(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Abrir archivo de procesos", "", "",
                               "Archivos de texto (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    
    try {
        m_processes = FileParser::ParseProcessesFile(openFileDialog.GetPath().ToStdString());
        
        if (m_processes.empty()) {
            wxMessageBox("No se encontraron procesos en el archivo seleccionado.", "Error", wxICON_ERROR);
            return;
        }
        
        wxString processesText;
        for (const auto& process : m_processes) {
            processesText += wxString::Format("%s, BT=%d, AT=%d, P=%d\n", 
                                            process->GetPID(), 
                                            process->GetBurstTime(), 
                                            process->GetArrivalTime(), 
                                            process->GetPriority());
        }
        
        m_processesText->SetValue(processesText);
        
        if (m_syncMechanism) {
            m_syncMechanism->LoadProcesses(m_processes);
        }
        
    } catch (const std::exception& e) {
        wxMessageBox(wxString::Format("Error al cargar el archivo: %s", e.what()), 
                    "Error", wxICON_ERROR);
    }
}

void SynchronizationPanel::OnLoadResources(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Abrir archivo de recursos", "", "",
                               "Archivos de texto (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    
    try {
        m_resources = FileParser::ParseResourcesFile(openFileDialog.GetPath().ToStdString());
        
        if (m_resources.empty()) {
            wxMessageBox("No se encontraron recursos en el archivo seleccionado.", "Error", wxICON_ERROR);
            return;
        }
        
        wxString resourcesText;
        for (const auto& pair : m_resources) {
            resourcesText += wxString::Format("%s, C=%d\n", 
                                            pair.second->GetName(), 
                                            pair.second->GetCounter());
        }
        
        m_resourcesText->SetValue(resourcesText);
        
        if (m_syncMechanism) {
            m_syncMechanism->LoadResources(m_resources);
        }
        
    } catch (const std::exception& e) {
        wxMessageBox(wxString::Format("Error al cargar el archivo: %s", e.what()), 
                    "Error", wxICON_ERROR);
    }
}

void SynchronizationPanel::OnLoadActions(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Abrir archivo de acciones", "", "",
                               "Archivos de texto (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    
    try {
        m_actions = FileParser::ParseActionsFile(openFileDialog.GetPath().ToStdString());
        
        if (m_actions.empty()) {
            wxMessageBox("No se encontraron acciones en el archivo seleccionado.", "Error", wxICON_ERROR);
            return;
        }
        
        wxString actionsText;
        for (const auto& action : m_actions) {
            actionsText += wxString::Format("%s, %s, %s, C=%d\n", 
                                          action->GetPID(), 
                                          (action->GetType() == ActionType::READ) ? "READ" : "WRITE", 
                                          action->GetResourceName(), 
                                          action->GetCycle());
        }
        
        m_actionsText->SetValue(actionsText);
        
        if (m_syncMechanism) {
            m_syncMechanism->LoadActions(m_actions);
        }
        
    } catch (const std::exception& e) {
        wxMessageBox(wxString::Format("Error al cargar el archivo: %s", e.what()), 
                    "Error", wxICON_ERROR);
    }
}

void SynchronizationPanel::OnStartSimulation(wxCommandEvent& event) {
    if (m_processes.empty() || m_resources.empty() || m_actions.empty()) {
        wxMessageBox("Es necesario cargar procesos, recursos y acciones antes de iniciar la simulación.", 
                    "Error", wxICON_ERROR);
        return;
    }
    
    m_isRunning = true;
    m_startButton->Disable();
    m_stopButton->Enable();
    m_resetButton->Disable();
    m_loadProcessesButton->Disable();
    m_loadResourcesButton->Disable();
    m_loadActionsButton->Disable();
    m_mechanismChoice->Disable();
    
    if (!m_timer.IsRunning()) {
        m_timer.SetOwner(this);
        m_timer.Start(500);  
    }
}

void SynchronizationPanel::OnStopSimulation(wxCommandEvent& event) {
    m_isRunning = false;
    m_startButton->Enable();
    m_stopButton->Disable();
    m_resetButton->Enable();
    
    if (m_timer.IsRunning()) {
        m_timer.Stop();
    }
}

void SynchronizationPanel::OnResetSimulation(wxCommandEvent& event) {
    m_isRunning = false;
    m_startButton->Enable();
    m_stopButton->Disable();
    m_loadProcessesButton->Enable();
    m_loadResourcesButton->Enable();
    m_loadActionsButton->Enable();
    m_mechanismChoice->Enable();
    
    if (m_timer.IsRunning()) {
        m_timer.Stop();
    }
    
    if (m_syncMechanism) {
        m_syncMechanism->Initialize();
    }
    
    m_ganttChart->Clear();
    
    m_cycleText->SetLabel("Ciclo: 0");
}

void SynchronizationPanel::OnTimer(wxTimerEvent& event) {
    if (!m_isRunning || !m_syncMechanism) {
        return;
    }
    
    m_syncMechanism->ExecuteCycle();
    
    auto events = m_syncMechanism->GetEvents();
    
    std::cout << "\n=== VISUALIZATION UPDATE ===" << std::endl;
    std::cout << "Total events received: " << events.size() << std::endl;
    for (size_t i = 0; i < events.size(); ++i) {
        const auto& evt = events[i];
        std::cout << "Event " << i << ": " << evt.pid << " -> " << evt.resourceName 
                  << " at cycle " << evt.cycle << " (waiting: " << (evt.isWaiting ? "YES" : "NO") << ")" << std::endl;
    }
    
    m_ganttChart->Clear();
    
    std::map<std::string, std::vector<SyncEvent>> groupedEvents;
    
    for (const auto& evt : events) {
        std::string key = evt.pid + "_" + evt.resourceName;
        groupedEvents[key].push_back(evt);
        std::cout << "Added event to group: " << key << " (cycle " << evt.cycle << ")" << std::endl;
    }
    
    std::cout << "Total groups created: " << groupedEvents.size() << std::endl;
    for (const auto& pair : groupedEvents) {
        std::cout << "Group '" << pair.first << "' has " << pair.second.size() << " events" << std::endl;
    }
    
    for (const auto& pair : groupedEvents) {
        const std::string& identifier = pair.first;
        const std::vector<SyncEvent>& eventList = pair.second;
        
        std::cout << "\nProcessing group: " << identifier << std::endl;
        
        size_t underscorePos = identifier.find('_');
        std::string pid = identifier.substr(0, underscorePos);
        std::string resourceName = identifier.substr(underscorePos + 1);
        
        std::cout << "Extracted - PID: '" << pid << "', Resource: '" << resourceName << "'" << std::endl;
        
        if (!eventList.empty()) {
            int startCycle = eventList[0].cycle;
            bool currentState = eventList[0].isWaiting;
            int duration = 1;
            
            std::cout << "Starting block: cycle " << startCycle << ", waiting: " << (currentState ? "YES" : "NO") << std::endl;
            
            for (size_t i = 1; i < eventList.size(); ++i) {
                if (eventList[i].isWaiting == currentState && 
                    eventList[i].cycle == startCycle + duration) {
                    duration++;
                    std::cout << "Extending block, new duration: " << duration << std::endl;
                } else {
                    std::cout << "Adding block: " << pid << "_" << resourceName 
                              << " at cycle " << startCycle << " duration " << duration 
                              << " waiting: " << (currentState ? "YES" : "NO") << std::endl;
                    
                    m_ganttChart->AddSyncBlock(pid, resourceName, startCycle, duration, currentState);
                    
                    startCycle = eventList[i].cycle;
                    currentState = eventList[i].isWaiting;
                    duration = 1;
                    
                    std::cout << "Starting new block: cycle " << startCycle << ", waiting: " << (currentState ? "YES" : "NO") << std::endl;
                }
            }
            
            std::cout << "Adding FINAL block: " << pid << "_" << resourceName 
                      << " at cycle " << startCycle << " duration " << duration 
                      << " waiting: " << (currentState ? "YES" : "NO") << std::endl;
            
            m_ganttChart->AddSyncBlock(pid, resourceName, startCycle, duration, currentState);
        }
    }
    
    int currentCycle = m_syncMechanism->GetCurrentCycle();
    std::cout << "Current cycle: " << currentCycle << std::endl;
    m_cycleText->SetLabel(wxString::Format("Ciclo: %d", currentCycle));
    m_ganttChart->UpdateCycle(currentCycle);
    
    std::cout << "=== END VISUALIZATION UPDATE ===\n" << std::endl;
    
    if (m_syncMechanism->IsComplete()) {
        std::cout << "Simulation complete! Final visualization updated." << std::endl;
        
        m_isRunning = false;
        m_startButton->Disable();
        m_stopButton->Disable();
        m_resetButton->Enable();
        m_loadProcessesButton->Enable();
        m_loadResourcesButton->Enable();
        m_loadActionsButton->Enable();
        m_mechanismChoice->Enable();
        
        if (m_timer.IsRunning()) {
            m_timer.Stop();
        }
        
        return;
    }
}

wxString SynchronizationPanel::GetResultsText() const {
    wxString results;
    
    results += "RESULTADOS DE LA SIMULACIÓN DE SINCRONIZACIÓN\n";
    results += "=======================================\n\n";
    
    results += "Mecanismo: " + m_mechanismChoice->GetStringSelection() + "\n";
    
    results += "\nProcesos:\n";
    results += m_processesText->GetValue();
    
    results += "\nRecursos:\n";
    results += m_resourcesText->GetValue();
    
    results += "\nAcciones:\n";
    results += m_actionsText->GetValue();
    
    results += "\nEventos:\n";
    if (m_syncMechanism) {
        auto events = m_syncMechanism->GetEvents();
        for (const auto& event : events) {
            results += wxString::Format("Proceso: %s, Recurso: %s, Acción: %s, Estado: %s, Ciclo: %d\n", 
                                      event.pid, 
                                      event.resourceName, 
                                      (event.actionType == ActionType::READ) ? "READ" : "WRITE", 
                                      event.isWaiting ? "WAITING" : "ACCESSED", 
                                      event.cycle);
        }
    }
    
    return results;
}