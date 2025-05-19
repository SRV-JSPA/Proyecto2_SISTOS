#include "GUI/SchedulerPanel.h"
#include "Scheduler/FIFOScheduler.h"
#include "Scheduler/SJFScheduler.h"
#include "Scheduler/SRTScheduler.h"
#include "Scheduler/RoundRobinScheduler.h"
#include "Scheduler/PriorityScheduler.h"
#include "Utils/FileParser.h"
#include <wx/filedlg.h>
#include <sstream>

wxBEGIN_EVENT_TABLE(SchedulerPanel, SimulationPanel)
    EVT_CHOICE(ID_AlgorithmChoice, SchedulerPanel::OnAlgorithmSelected)
    EVT_BUTTON(ID_LoadProcessesButton, SchedulerPanel::OnLoadProcesses)
wxEND_EVENT_TABLE()

SchedulerPanel::SchedulerPanel(wxWindow* parent)
    : SimulationPanel(parent) {
    
    wxStaticText* algorithmLabel = new wxStaticText(this, wxID_ANY, "Algoritmo:");
    
    wxArrayString choices;
    choices.Add("First In First Out (FIFO)");
    choices.Add("Shortest Job First (SJF)");
    choices.Add("Shortest Remaining Time (SRT)");
    choices.Add("Round Robin (RR)");
    choices.Add("Priority");
    
    m_algorithmChoice = new wxChoice(this, ID_AlgorithmChoice, wxDefaultPosition, wxDefaultSize, choices);
    m_algorithmChoice->SetSelection(0);
    
    m_quantumLabel = new wxStaticText(this, wxID_ANY, "Quantum:");
    m_quantumSpin = new wxSpinCtrl(this, ID_QuantumSpin, wxEmptyString, wxDefaultPosition, wxDefaultSize, 
                                   wxSP_ARROW_KEYS, 1, 100, 1);
    
    m_quantumLabel->Hide();
    m_quantumSpin->Hide();
    
    m_loadProcessesButton = new wxButton(this, ID_LoadProcessesButton, "Cargar Procesos");
    
    wxStaticBoxSizer* processesSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Procesos");
    m_processesText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 
                                    wxTE_MULTILINE | wxTE_READONLY);
    processesSizer->Add(m_processesText, 1, wxEXPAND | wxALL, 5);
    
    wxStaticBoxSizer* metricsSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Métricas");
    m_metricsText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 
                                  wxTE_MULTILINE | wxTE_READONLY);
    metricsSizer->Add(m_metricsText, 1, wxEXPAND | wxALL, 5);
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* configSizer = new wxBoxSizer(wxHORIZONTAL);
    configSizer->Add(algorithmLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    configSizer->Add(m_algorithmChoice, 0, wxALL, 5);
    configSizer->Add(m_quantumLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    configSizer->Add(m_quantumSpin, 0, wxALL, 5);
    configSizer->Add(m_loadProcessesButton, 0, wxALL, 5);
    
    mainSizer->Add(configSizer, 0, wxEXPAND);
    mainSizer->Add(m_startButton, 0, wxALL, 5);
    mainSizer->Add(m_stopButton, 0, wxALL, 5);
    mainSizer->Add(m_resetButton, 0, wxALL, 5);
    mainSizer->Add(m_cycleText, 0, wxALL, 5);
    
    wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);
    contentSizer->Add(processesSizer, 1, wxEXPAND | wxALL, 5);
    contentSizer->Add(metricsSizer, 1, wxEXPAND | wxALL, 5);
    
    mainSizer->Add(contentSizer, 0, wxEXPAND);
    mainSizer->Add(m_ganttChart, 1, wxEXPAND | wxALL, 5);
    
    SetSizer(mainSizer);
    
    CreateScheduler();
}

void SchedulerPanel::CreateScheduler() {
    int selection = m_algorithmChoice->GetSelection();
    
    switch (selection) {
        case 0: 
            m_scheduler = std::make_shared<FIFOScheduler>();
            break;
        case 1: 
            m_scheduler = std::make_shared<SJFScheduler>();
            break;
        case 2: 
            m_scheduler = std::make_shared<SRTScheduler>();
            break;
        case 3: 
            m_scheduler = std::make_shared<RoundRobinScheduler>(m_quantumSpin->GetValue());
            break;
        case 4: 
            m_scheduler = std::make_shared<PriorityScheduler>();
            break;
        default:
            m_scheduler = std::make_shared<FIFOScheduler>();
            break;
    }
    
    if (!m_processes.empty()) {
        m_scheduler->LoadProcesses(m_processes);
        m_scheduler->Initialize();
    }
}

void SchedulerPanel::UpdateMetrics() {
    if (m_scheduler) {
        double avgWaitingTime = m_scheduler->CalculateAverageWaitingTime();
        wxString metricsText = wxString::Format("Tiempo de espera promedio: %.2f ciclos", avgWaitingTime);
        m_metricsText->SetValue(metricsText);
    }
}

void SchedulerPanel::OnAlgorithmSelected(wxCommandEvent& event) {
    int selection = m_algorithmChoice->GetSelection();
    
    if (selection == 3) {  
        m_quantumLabel->Show();
        m_quantumSpin->Show();
    } else {
        m_quantumLabel->Hide();
        m_quantumSpin->Hide();
    }
    
    GetSizer()->Layout();
    
    CreateScheduler();
}

void SchedulerPanel::OnLoadProcesses(wxCommandEvent& event) {
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
        
        if (m_scheduler) {
            m_scheduler->LoadProcesses(m_processes);
            m_scheduler->Initialize();
        }
        
        m_startButton->Enable();
        
    } catch (const std::exception& e) {
        wxMessageBox(wxString::Format("Error al cargar el archivo: %s", e.what()), 
                    "Error", wxICON_ERROR);
    }
}

void SchedulerPanel::OnStartSimulation(wxCommandEvent& event) {
    if (m_processes.empty()) {
        wxMessageBox("No hay procesos cargados. Por favor, cargue procesos primero.", 
                    "Error", wxICON_ERROR);
        return;
    }
    
    m_isRunning = true;
    m_startButton->Disable();
    m_stopButton->Enable();
    m_resetButton->Disable();
    m_loadProcessesButton->Disable();
    m_algorithmChoice->Disable();
    m_quantumSpin->Disable();
    
    if (!m_timer.IsRunning()) {
        m_timer.SetOwner(this);
        m_timer.Start(500);  
    }
}

void SchedulerPanel::OnStopSimulation(wxCommandEvent& event) {
    m_isRunning = false;
    m_startButton->Enable();
    m_stopButton->Disable();
    m_resetButton->Enable();
    
    if (m_timer.IsRunning()) {
        m_timer.Stop();
    }
}

void SchedulerPanel::OnResetSimulation(wxCommandEvent& event) {
    m_isRunning = false;
    m_startButton->Enable();
    m_stopButton->Disable();
    m_loadProcessesButton->Enable();
    m_algorithmChoice->Enable();
    m_quantumSpin->Enable();
    
    if (m_timer.IsRunning()) {
        m_timer.Stop();
    }
    
    CreateScheduler();
    
    m_ganttChart->Clear();
    
    m_cycleText->SetLabel("Ciclo: 0");
    
    m_metricsText->Clear();
}

void SchedulerPanel::OnTimer(wxTimerEvent& event) {
    if (!m_isRunning || !m_scheduler) {
        return;
    }
    
    if (m_scheduler->IsComplete()) {
        m_isRunning = false;
        m_startButton->Disable();
        m_stopButton->Disable();
        m_resetButton->Enable();
        m_loadProcessesButton->Enable();
        m_algorithmChoice->Enable();
        m_quantumSpin->Enable();
        
        if (m_timer.IsRunning()) {
            m_timer.Stop();
        }
        
        UpdateMetrics();
        
        return;
    }
    
    m_scheduler->ExecuteCycle();
    
    auto events = m_scheduler->GetEvents();
    m_ganttChart->Clear();
    
    for (const auto& event : events) {
        m_ganttChart->AddProcessBlock(event.pid, event.startCycle, event.duration);
    }
    
    m_cycleText->SetLabel(wxString::Format("Ciclo: %d", m_scheduler->GetCurrentCycle()));
    
    m_ganttChart->UpdateCycle(m_scheduler->GetCurrentCycle());
    
    UpdateMetrics();
}

wxString SchedulerPanel::GetResultsText() const {
    wxString results;
    
    results += "RESULTADOS DE LA SIMULACIÓN\n";
    results += "==========================\n\n";
    
    results += "Algoritmo: " + m_algorithmChoice->GetStringSelection() + "\n";
    
    if (m_algorithmChoice->GetSelection() == 3) {  
        results += wxString::Format("Quantum: %d\n", m_quantumSpin->GetValue());
    }
    
    results += "\nProcesos:\n";
    results += m_processesText->GetValue();
    
    results += "\nMétricas:\n";
    results += m_metricsText->GetValue();
    
    results += "\nEventos:\n";
    if (m_scheduler) {
        auto events = m_scheduler->GetEvents();
        for (const auto& event : events) {
            results += wxString::Format("Proceso: %s, Inicio: %d, Duración: %d\n", 
                                      event.pid, event.startCycle, event.duration);
        }
    }
    
    return results;
}