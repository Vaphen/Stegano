#include "GUI.h"


#include <wx/string.h>
#include <wx/intl.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/gbsizer.h>
#include <wx/wx.h>
#include "include/Strings.h"

#define ID_Hello 1
#define ID_Start 2


BEGIN_EVENT_TABLE(GUI,wxFrame)
    EVT_MENU(ID_Hello,   GUI::OnHello)
    EVT_MENU(wxID_EXIT,  GUI::OnExit)
    EVT_MENU(wxID_ABOUT, GUI::OnAbout)
    EVT_MENU(ID_Start, GUI::OnStart)
END_EVENT_TABLE()

GUI::GUI()
{
	Create(nullptr, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
	SetMinSize(wxSize(500, 650));
	SetMaxSize(wxSize(500, 650));
	SetWindowStyleFlag(wxWINDOW_VARIANT_MAX);
	SetTitle(std::string("Stegano+ V.").append(VERSION));
	Center();

    createMenuBar();

    wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText *headerLabel = new wxStaticText(this, wxID_ANY, "Stegano+", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    wxFont *headerFont = new wxFont(30, wxMODERN, wxNORMAL, wxNORMAL);
    headerLabel->SetFont(*headerFont);
    verticalSizer->Add(headerLabel, 0, wxCENTER, 0);

    wxGridBagSizer *form = new wxGridBagSizer(20, 10);
    wxBitmap *openDialogButtonIcon = new wxBitmap("./res/folder.png", wxBITMAP_TYPE_PNG);
   // wxFont *descriptorFont = new wxFont(10, wxNORMAL, wxNORMAL, wxNORMAL);
    wxStaticText *containerFileLabel = new wxStaticText(this, wxID_ANY, "Kontainer-Datei: ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
 //   containerFileLabel->SetFont(*descriptorFont);
    wxTextCtrl *containerFileInput = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(240, -1), wxALIGN_LEFT);
    containerFileInput->SetEditable(false);
    containerFileInput->Disable();

    wxBitmapButton *openContainerDialogButton = new wxBitmapButton(this, wxID_ANY, *openDialogButtonIcon, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    wxRadioButton *hideFileRadio = new wxRadioButton(this, wxID_ANY, "Datei verstecken");

    wxStaticText *hideFileLabel = new wxStaticText(this, wxID_ANY, "zu versteckende Datei: ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
 //   containerFileLabel->SetFont(*descriptorFont);
    wxTextCtrl *hideFileInput = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(240, -1), wxALIGN_LEFT);
    hideFileInput->SetEditable(false);
    hideFileInput->Disable();
    wxBitmapButton *openHideFileDialogButton = new wxBitmapButton(this, wxID_ANY, *openDialogButtonIcon, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    wxRadioButton *hideTextRadio = new wxRadioButton(this, wxID_ANY, "Text verstecken");

    wxTextCtrl *textToHideCtrl = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(440, 200), wxTE_MULTILINE | wxNO_BORDER);

    wxButton *startHidingButton = new wxButton(this, wxID_ANY, _T("Start!"), wxDefaultPosition, wxSize(200, 40));

    form->Add(containerFileLabel, wxGBPosition(0, 0));
    form->Add(containerFileInput, wxGBPosition(0, 1));
    form->Add(openContainerDialogButton, wxGBPosition(0, 2));

    form->Add(hideFileRadio, wxGBPosition(1, 0));
    form->Add(new wxStaticText(this, wxID_ANY, _T("")), wxGBPosition(1, 1));
    form->Add(new wxStaticText(this, wxID_ANY, _T("")), wxGBPosition(1, 2));

    form->Add(hideFileLabel, wxGBPosition(2, 0));
    form->Add(hideFileInput, wxGBPosition(2, 1));
    form->Add(openHideFileDialogButton, wxGBPosition(2, 2));

    form->Add(hideTextRadio, wxGBPosition(3, 0));
    form->Add(new wxStaticText(this, wxID_ANY, _T("")), wxGBPosition(3, 1));
    form->Add(new wxStaticText(this, wxID_ANY, _T("")), wxGBPosition(3, 2));


    form->Add(textToHideCtrl, wxGBPosition(4, 0), wxGBSpan(1, 3));

    form->Add(startHidingButton, wxGBPosition(5, 0), wxGBSpan(1, 3));
    verticalSizer->Add(form, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 30);


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

void GUI::OnStart(wxCommandEvent &event) {
  //  Stegano
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
