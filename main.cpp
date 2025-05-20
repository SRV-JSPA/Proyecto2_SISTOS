#include <wx/wx.h>
#include "include/GUI/MainFrame.h"

class SimulatorApp : public wxApp {
public:
    virtual bool OnInit() {
        MainFrame* frame = new MainFrame("Simulador de Sistemas Operativos");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(SimulatorApp);