#include "GUI/MainFrame.h"
#include <wx/file.h> 

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(ID_Quit, MainFrame::OnQuit)
    EVT_MENU(ID_About, MainFrame::OnAbout)
    EVT_MENU(ID_SaveResults, MainFrame::OnSaveResults)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
    
    CreateMenuBar();
    
    CreateStatusBar(); 
    SetStatusText("Listo");
    
    m_notebook = new wxNotebook(this, wxID_ANY);
    
    m_schedulerPanel = new SchedulerPanel(m_notebook);
    m_syncPanel = new SynchronizationPanel(m_notebook);
    
    m_notebook->AddPage(m_schedulerPanel, "Algoritmos de Calendarización");
    m_notebook->AddPage(m_syncPanel, "Mecanismos de Sincronización");
    
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_notebook, 1, wxEXPAND | wxALL, 5);
    SetSizer(sizer);
    
    Centre();
    
    Show(true);
}

void MainFrame::CreateMenuBar() {
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_SaveResults, "&Guardar resultados\tCtrl+S", "Guardar resultados de la simulación");
    menuFile->AppendSeparator();
    menuFile->Append(ID_Quit, "&Salir\tAlt+F4", "Cerrar la aplicación");
    
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(ID_About, "&Acerca de...\tF1", "Mostrar información sobre la aplicación");
    
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&Archivo");
    menuBar->Append(menuHelp, "&Ayuda");
    
    SetMenuBar(menuBar);
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
    wxMessageBox("Simulador de Sistemas Operativos\nDesarrollado para el curso de Sistemas Operativos\nUniversidad del Valle de Guatemala", 
                 "Acerca de Simulador", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnSaveResults(wxCommandEvent& WXUNUSED(event)) {
    wxFileDialog saveFileDialog(this, "Guardar resultados", "", "",
                               "Archivos de texto (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    
    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    
    wxFile file(saveFileDialog.GetPath(), wxFile::write);
    
    if (!file.IsOpened()) {
        wxLogError("No se pudo guardar en el archivo '%s'", saveFileDialog.GetPath());
        return;
    }
    
    wxString resultsText;
    
    if (m_notebook->GetSelection() == 0) {
        resultsText = m_schedulerPanel->GetResultsText();
    } else {
        resultsText = m_syncPanel->GetResultsText();
    }
    
    file.Write(resultsText);
    file.Close();
    
    SetStatusText("Resultados guardados correctamente");
}