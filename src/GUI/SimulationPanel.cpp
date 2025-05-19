#include "GUI/SimulationPanel.h"

wxBEGIN_EVENT_TABLE(SimulationPanel, wxPanel)
    EVT_TIMER(wxID_ANY, SimulationPanel::OnTimer)
wxEND_EVENT_TABLE()

SimulationPanel::SimulationPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY), m_isRunning(false) {
    
    m_startButton = new wxButton(this, wxID_ANY, "Iniciar");
    m_stopButton = new wxButton(this, wxID_ANY, "Detener");
    m_resetButton = new wxButton(this, wxID_ANY, "Reiniciar");
    m_cycleText = new wxStaticText(this, wxID_ANY, "Ciclo: 0");
    m_ganttChart = new GanttChartPanel(this);
    
    m_stopButton->Disable();
    
    m_startButton->Bind(wxEVT_BUTTON, &SimulationPanel::OnStartSimulation, this);
    m_stopButton->Bind(wxEVT_BUTTON, &SimulationPanel::OnStopSimulation, this);
    m_resetButton->Bind(wxEVT_BUTTON, &SimulationPanel::OnResetSimulation, this);
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* controlSizer = new wxBoxSizer(wxHORIZONTAL);
    controlSizer->Add(m_startButton, 0, wxALL, 5);
    controlSizer->Add(m_stopButton, 0, wxALL, 5);
    controlSizer->Add(m_resetButton, 0, wxALL, 5);
    controlSizer->Add(m_cycleText, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    
    mainSizer->Add(controlSizer, 0, wxEXPAND);
    mainSizer->Add(m_ganttChart, 1, wxEXPAND | wxALL, 5);
    
    SetSizer(mainSizer);
}

SimulationPanel::~SimulationPanel() {
    if (m_timer.IsRunning()) {
        m_timer.Stop();
    }
}