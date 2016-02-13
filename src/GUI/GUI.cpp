#include "GUI.h"


#include <fstream>
#include "Strings.h"


BEGIN_EVENT_TABLE(GUI,wxFrame)
    EVT_MENU(wxID_EXIT,  GUI::OnExit)
    EVT_MENU(wxID_ABOUT, GUI::OnAbout)
END_EVENT_TABLE()

GUI::GUI()
{
    Create(nullptr, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    initializeWindow();

    // add a menu bar to the window
    createMenuBar()
    ;

    // set status bar
    CreateStatusBar();
    SetStatusText(std::string("Stegano+ V.").append(VERSION));

    // Header
    wxBoxSizer *headerSizer = new wxBoxSizer(wxHORIZONTAL);
    ImagePanel *logoPanel = new ImagePanel(this, "res/logo_small.png", wxBITMAP_TYPE_PNG);
    wxStaticText *headerLabel = new wxStaticText(this, wxID_ANY, "Stegano+", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    wxFont *headerFont = new wxFont(30, wxMODERN, wxNORMAL, wxNORMAL);
    headerLabel->SetFont(*headerFont);

    headerSizer->Add(logoPanel, 0, wxALIGN_CENTER, 0);
    headerSizer->Add(headerLabel, 0, wxALIGN_CENTER, 0);

    // main widgets
    MainPanel *notebook = new MainPanel(this);
    wxBoxSizer *mySizer = new wxBoxSizer(wxVERTICAL);
    mySizer->Add(headerSizer, 0, wxALIGN_CENTER, 0);
    mySizer->Add(notebook, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 0);

    // set main sizer
    this->SetSizer(mySizer);

    // close event
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&GUI::OnClose);
}

GUI::~GUI()
{

}

/** \brief Set the window properties
 */
void GUI::initializeWindow()
{
    SetMinSize(wxSize(550, 750));
    SetMaxSize(wxSize(550, 750));
    SetWindowStyleFlag(wxWINDOW_VARIANT_MAX);
    SetTitle(std::string("Stegano+ V.").append(VERSION));
    Center();
}

/** \brief Creates the menu bar with its items and adds it to the main window
 */
void GUI::createMenuBar()
{
    wxMenu *menuFile = new wxMenu;
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
}


/** \brief Triggered when the gui closes
 * \param event wxCloseEvent& the close event
 */
void GUI::OnClose(wxCloseEvent& event)
{
    Destroy();
}


/** \brief Show short "about"-message
 * \param event wxCommandEvent& the event the clicked widget triggered
 */
void GUI::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(_T(ABOUT_MESSAGE),
                 _T(ABOUT_TITLE),
                 wxOK | wxICON_INFORMATION);
}


/** \brief Called if e.g. close in the menu bar is clicked.
 * \param event wxCommandEvent& the event the clicked widget triggered
 */
void GUI::OnExit(wxCommandEvent& event)
{
    Close(true);
}

