#include "GUI.h"


#include <fstream>
#include "include/Strings.h"

#define ID_OpenFile 1
#define ID_SelectOutputFile 2
#define ID_SelectContainerFile 3
#define ID_SelectHideFile 4
#define ID_HideFileRadio 5
#define ID_HideTextRadio 6
#define ID_Start 7


BEGIN_EVENT_TABLE(GUI,wxFrame)
    EVT_MENU(ID_OpenFile,   GUI::OnOpenFileClicked)
    EVT_BUTTON(ID_SelectOutputFile, GUI::OnSelectOutputFileClicked)
    EVT_BUTTON(ID_SelectContainerFile, GUI::OnOpenFileClicked)
    EVT_BUTTON(ID_SelectHideFile, GUI::OnOpenFileClicked)
    EVT_RADIOBUTTON(ID_HideFileRadio, GUI::HideRadioChanged)
    EVT_RADIOBUTTON(ID_HideTextRadio, GUI::HideRadioChanged)
    EVT_MENU(wxID_EXIT,  GUI::OnExit)
    EVT_MENU(wxID_ABOUT, GUI::OnAbout)
    EVT_BUTTON(ID_Start, GUI::OnStart)
END_EVENT_TABLE()

GUI::GUI()
{
	Create(nullptr, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
	SetMinSize(wxSize(500, 700));
	SetMaxSize(wxSize(500, 700));
	SetWindowStyleFlag(wxWINDOW_VARIANT_MAX);
	SetTitle(std::string("Stegano+ V.").append(VERSION));
	Center();

    createMenuBar();

    // Header
    wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText *headerLabel = new wxStaticText(this, wxID_ANY, "Stegano+", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    wxFont *headerFont = new wxFont(30, wxMODERN, wxNORMAL, wxNORMAL);
    headerLabel->SetFont(*headerFont);
    verticalSizer->Add(headerLabel, 0, wxCENTER, 0);

    //***********
    // Main-Part
    //***********
    // global used widgets
    wxGridBagSizer *form = new wxGridBagSizer(20, 10);
    wxBitmap *openDialogButtonIcon = new wxBitmap("./res/folder.png", wxBITMAP_TYPE_PNG);


    // Output-File
    wxStaticText *outputFileLabel = new wxStaticText(this, wxID_ANY, "Ausgabe-Datei: ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    outputFileInput = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(240, -1), wxALIGN_LEFT);
    outputFileInput->Disable();
    wxBitmapButton *openOutputDialogButton = new wxBitmapButton(this, ID_SelectOutputFile, *openDialogButtonIcon, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

    // Container File
    wxStaticText *containerFileLabel = new wxStaticText(this, wxID_ANY, "Kontainer-Datei: ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    containerFileInput = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(240, -1), wxALIGN_LEFT);
    containerFileInput->Disable();
    openContainerDialogButton = new wxBitmapButton(this, ID_SelectContainerFile, *openDialogButtonIcon, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

    // File-Radio
    wxRadioButton *hideFileRadio = new wxRadioButton(this, ID_HideFileRadio, "Datei verstecken");

    // Hide File
    wxStaticText *hideFileLabel = new wxStaticText(this, wxID_ANY, "zu versteckende Datei: ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    hideFileInput = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(240, -1), wxALIGN_LEFT);
    hideFileInput->Disable();
    openHideFileDialogButton = new wxBitmapButton(this, ID_SelectHideFile, *openDialogButtonIcon, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

    // Text-Radio
    wxRadioButton *hideTextRadio = new wxRadioButton(this, ID_HideTextRadio, "Text verstecken");

    // Hide Text
    textToHideCtrl = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(440, 200), wxTE_MULTILINE);
    textToHideCtrl->Enable(false);

    // Start button
    wxButton *startHidingButton = new wxButton(this, ID_Start, _T("Start!"), wxDefaultPosition, wxSize(200, 40));

    form->Add(outputFileLabel, wxGBPosition(0, 0));
    form->Add(outputFileInput, wxGBPosition(0, 1));
    form->Add(openOutputDialogButton, wxGBPosition(0, 2));

    form->Add(containerFileLabel, wxGBPosition(1, 0));
    form->Add(containerFileInput, wxGBPosition(1, 1));
    form->Add(openContainerDialogButton, wxGBPosition(1, 2));

    form->Add(hideFileRadio, wxGBPosition(2, 0));
    form->Add(new wxStaticText(this, wxID_ANY, _T("")), wxGBPosition(2, 1));
    form->Add(new wxStaticText(this, wxID_ANY, _T("")), wxGBPosition(2, 2));

    form->Add(hideFileLabel, wxGBPosition(3, 0));
    form->Add(hideFileInput, wxGBPosition(3, 1));
    form->Add(openHideFileDialogButton, wxGBPosition(3, 2));

    form->Add(hideTextRadio, wxGBPosition(4, 0));
    form->Add(new wxStaticText(this, wxID_ANY, _T("")), wxGBPosition(4, 1));
    form->Add(new wxStaticText(this, wxID_ANY, _T("")), wxGBPosition(4, 2));


    form->Add(textToHideCtrl, wxGBPosition(5, 0), wxGBSpan(1, 3));

    form->Add(startHidingButton, wxGBPosition(6, 0), wxGBSpan(1, 3));
    verticalSizer->Add(form, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 30);


    SetSizer(verticalSizer);

	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&GUI::OnClose);

}

GUI::~GUI()
{

}

void GUI::createMenuBar() {
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_OpenFile,
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
    std::string outputFilePath = outputFileInput->GetValue().ToStdString();
    std::string containerFilePath = containerFileInput->GetValue().ToStdString();
    std::string hideFilePath = hideFileInput->GetValue().ToStdString();

    if(outputFilePath.empty() || containerFilePath.empty() || hideFilePath.empty()) {
        ShowErrorDialog("Steganographie war nicht möglich", "Ein benötigtes Feld wurde nicht hinreichend ausgefüllt.");
        return;
    }

    if(outputFilePath.find_last_of(".png") != outputFilePath.size() - 4 &&
       outputFilePath.find_last_of("PNG") != outputFilePath.size() - 4) {
        outputFilePath += ".png";
       }

    SteganoHide hider;

    try {
        hider.loadPicture(containerFilePath);
    } catch(SteganoException &ex) {
        ShowErrorDialog("Es ist ein Fehler aufgetreten", ex.what());
    }

    std::ifstream hideFileStream(hideFilePath, std::fstream::binary);
    hider.setOutputFilePath(outputFilePath);
    hider.hideFile(hideFileStream, "notusedpassword");
    hider.saveChangesToDisk();
    hideFileStream.close();
}

void GUI::OnOpenFileClicked(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, _("Open XYZ file"), "", "", "Picture files (*.jpg)|*.jpg", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    // user cancelled his action
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    std::string userInputPath = openFileDialog.GetPath().ToStdString();
    std::ifstream userChosenFileStream(userInputPath);
    if(!userChosenFileStream.good()) {
        return;
    }
    if(event.GetId() == ID_SelectHideFile) {
        hideFileInput->ChangeValue(userInputPath);
    } else {
        containerFileInput->ChangeValue(userInputPath);
    }
    userChosenFileStream.close();
}

void GUI::OnSelectOutputFileClicked(wxCommandEvent &event) {
    wxFileDialog openFileDialog(this, _("Open XYZ file"), "", "", "PortableNetworkGraphics files (*.png)|*.png", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    // user cancelled his action
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    std::string userInputPath = openFileDialog.GetPath().ToStdString();
    std::ofstream userChosenFileStream(userInputPath, std::ofstream::app);
    if(!userChosenFileStream.good()) {
        return;
    }
    outputFileInput->ChangeValue(userInputPath);
    userChosenFileStream.close();
}

void GUI::HideRadioChanged(wxCommandEvent& event) {
    if(event.GetId() == ID_HideTextRadio) {
        openHideFileDialogButton->Enable(false);
        textToHideCtrl->Enable(true);
    } else {
        openHideFileDialogButton->Enable(true);
        textToHideCtrl->Enable(false);
    }
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


// TODO: this is somehow not working
void GUI::ShowErrorDialog(const wxString &title, const wxString &message) {
  wxMessageDialog *dial = new wxMessageDialog(this, message, title, wxOK|wxICON_ERROR);
  dial->ShowModal();
}
