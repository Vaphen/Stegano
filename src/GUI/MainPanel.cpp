#include "MainPanel.h"

MainPanel::MainPanel(wxWindow *parent) : wxNotebook(parent, wxID_ANY)
{
    HidePage *hidePage = new HidePage(this, wxID_ANY);
    ExposePage *exposePage = new ExposePage(this, wxID_ANY);
    this->AddPage(hidePage, "Daten verstecken", true);
    this->AddPage(exposePage, "Daten extrahieren", false);
    //this->AddPage(, "Daten extrahieren", false);
}

MainPanel::~MainPanel()
{

}
