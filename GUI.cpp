#include "GUI.h"


#include <wx/string.h>
#include <wx/intl.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include "include/Strings.h"

#define ID_Hello 1


BEGIN_EVENT_TABLE(GUI,wxFrame)
    EVT_MENU(ID_Hello,   GUI::OnHello)
    EVT_MENU(wxID_EXIT,  GUI::OnExit)
    EVT_MENU(wxID_ABOUT, GUI::OnAbout)
END_EVENT_TABLE()

GUI::GUI()
{
	Create(nullptr, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
	SetMinSize(wxSize(500, 500));
	SetMaxSize(wxSize(500, 500));
	SetWindowStyleFlag(wxWINDOW_VARIANT_MAX);
	SetTitle(std::string("Stegano+ V.").append(VERSION));
	Center();

    createMenuBar();

    wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText *headerLabel = new wxStaticText(this, wxID_ANY, "Stegano+", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    wxFont *headerFont = new wxFont(30, wxMODERN, wxNORMAL, wxNORMAL);
    headerLabel->SetFont(*headerFont);
    verticalSizer->Add(headerLabel, 0, wxCENTER, 0);
    SetSizer(verticalSizer);

	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&GUI::OnClose);

}

GUI::~GUI()
{

}

void GUI::createMenuBar() {
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello,
                     _T(MENU_ITEM_OPEN),
                     _T(MENU_ITEM_OPEN_HELP));
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT,
                     _T(MENU_ITEM_EXIT),
                     _T(MENU_ITEM_EXIT_HELP));
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT,
                     _T(MENU_ITEM_ABOUT),
                     _T(MENU_ITEM_ABOUT_HELP));
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, _T(MENU_ITEM_FILE) );
    menuBar->Append( menuHelp, _T(MENU_ITEM_HELP) );
    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText(std::string("Stegano+ V.").append(VERSION));
}

void GUI::OnHello(wxCommandEvent& event) {
    Close( true );
}

void GUI::OnAbout(wxCommandEvent& event) {
        wxMessageBox(_T(ABOUT_MESSAGE),
                     _T(ABOUT_TITLE),
                     wxOK | wxICON_INFORMATION);
}

void GUI::OnExit(wxCommandEvent& event) {
     Close(true);
}

void GUI::OnClose(wxCloseEvent& event) {
    Destroy();
}
