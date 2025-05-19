#pragma once
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/file.h> 
#include "GUI/SchedulerPanel.h"
#include "GUI/SynchronizationPanel.h"

enum {
    ID_Quit = wxID_EXIT,
    ID_About = wxID_ABOUT,
    ID_SaveResults = wxID_SAVE,
    ID_SchedulerPanel = wxID_HIGHEST + 1,
    ID_SyncPanel
};

class MainFrame : public wxFrame {
private:
    wxNotebook* m_notebook;
    SchedulerPanel* m_schedulerPanel;
    SynchronizationPanel* m_syncPanel;
    
    void CreateMenuBar();    
public:
    MainFrame(const wxString& title);
    
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnSaveResults(wxCommandEvent& event);
    
    wxDECLARE_EVENT_TABLE();
};