#include "HidePage.h"

BEGIN_EVENT_TABLE(HidePage, wxNotebookPage)
    EVT_BUTTON(ID_SelectOutputFile, HidePage::OnSelectOutputFileClicked)
    EVT_BUTTON(ID_SelectContainerFile, HidePage::OnOpenFileClicked)
    EVT_BUTTON(ID_SelectHideFile, HidePage::OnOpenFileClicked)
    EVT_RADIOBUTTON(ID_HideFileRadio, HidePage::HideRadioChanged)
    EVT_RADIOBUTTON(ID_HideTextRadio, HidePage::HideRadioChanged)
    EVT_BUTTON(ID_Start, HidePage::OnStart)
END_EVENT_TABLE()

HidePage::HidePage(wxWindow *parent, int widgetID) : wxNotebookPage(parent, widgetID)
{
    wxBoxSizer *hideSizer = new wxBoxSizer(wxVERTICAL);

    //***********
    // Main-Part
    //***********
    // global used widgets
    form = new wxGridBagSizer(20, 10);
    openDialogButtonIcon = new wxBitmap("./res/folder.png", wxBITMAP_TYPE_PNG);

    addOutputFileArea();

    addContainerFileArea();

    // File-Radio
    wxRadioButton *hideFileRadio = new wxRadioButton(this, ID_HideFileRadio, "Datei verstecken");
    form->Add(hideFileRadio, wxGBPosition(2, 0));
    form->Add(new wxStaticText(this, wxID_ANY, _T("")), wxGBPosition(2, 1));
    form->Add(new wxStaticText(this, wxID_ANY, _T("")), wxGBPosition(2, 2));

    addFileToHideArea();

    // Text-Radio
    wxRadioButton *hideTextRadio = new wxRadioButton(this, ID_HideTextRadio, "Text verstecken");
    form->Add(hideTextRadio, wxGBPosition(4, 0));
    form->Add(new wxStaticText(this, wxID_ANY, _T("")), wxGBPosition(4, 1));
    form->Add(new wxStaticText(this, wxID_ANY, _T("")), wxGBPosition(4, 2));

    // Hide Text
    addPhraseToHideArea();

    // Start button and progressbar
    startHidingButton = new wxButton(this, ID_Start, _T("Start!"), wxDefaultPosition);
    progressBar = new wxGauge(this, wxID_ANY, 100);

    form->Add(startHidingButton, wxGBPosition(6, 0), wxGBSpan(1, 1), wxEXPAND);
    form->Add(progressBar, wxGBPosition(6, 1), wxGBSpan(1, 2), wxEXPAND);

    hideSizer->Add(form, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 30);

    SetSizer(hideSizer);
}

HidePage::~HidePage() { }

void HidePage::addOutputFileArea() {
    wxStaticText *outputFileLabel = new wxStaticText(this, wxID_ANY, "Ausgabe-Datei: ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    outputFileInput = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(240, -1), wxALIGN_LEFT);
    outputFileInput->Disable();
    wxBitmapButton *openOutputDialogButton = new wxBitmapButton(this, ID_SelectOutputFile, *openDialogButtonIcon);

    form->Add(outputFileLabel, wxGBPosition(0, 0));
    form->Add(outputFileInput, wxGBPosition(0, 1));
    form->Add(openOutputDialogButton, wxGBPosition(0, 2));
}

void HidePage::addContainerFileArea() {
    wxStaticText *containerFileLabel = new wxStaticText(this, wxID_ANY, "Kontainer-Datei: ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    containerFileInput = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(240, -1), wxALIGN_LEFT);
    containerFileInput->Disable();
    openContainerDialogButton = new wxBitmapButton(this, ID_SelectContainerFile, *openDialogButtonIcon);

    form->Add(containerFileLabel, wxGBPosition(1, 0));
    form->Add(containerFileInput, wxGBPosition(1, 1));
    form->Add(openContainerDialogButton, wxGBPosition(1, 2));
}

void HidePage::addFileToHideArea() {
    wxStaticText *hideFileLabel = new wxStaticText(this, wxID_ANY, "zu versteckende Datei: ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    hideFileInput = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(240, -1), wxALIGN_LEFT);
    hideFileInput->Disable();
    openHideFileDialogButton = new wxBitmapButton(this, ID_SelectHideFile, *openDialogButtonIcon, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

    form->Add(hideFileLabel, wxGBPosition(3, 0));
    form->Add(hideFileInput, wxGBPosition(3, 1));
    form->Add(openHideFileDialogButton, wxGBPosition(3, 2));
}

void HidePage::addPhraseToHideArea() {
    textToHideCtrl = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(440, 200), wxTE_MULTILINE);
    textToHideCtrl->Enable(false);

     form->Add(textToHideCtrl, wxGBPosition(5, 0), wxGBSpan(1, 3));
}

void HidePage::OnStart(wxCommandEvent &event) {
    std::string outputFilePath = outputFileInput->GetValue().ToStdString();
    std::string containerFilePath = containerFileInput->GetValue().ToStdString();
    std::string hideFilePath = hideFileInput->GetValue().ToStdString();

    if(outputFilePath.empty() || containerFilePath.empty() || hideFilePath.empty()) {
        ShowErrorDialog(_T("Steganographie war nicht möglich"), _T("Ein benötigtes Feld wurde nicht hinreichend ausgefüllt."));
        return;
    }

    if(outputFilePath.substr(outputFilePath.find_last_of(".") + 1) != "png" &&
       outputFilePath.substr(outputFilePath.find_last_of(".") + 1) != "PNG") {
        outputFilePath += ".png";
       }

    try {
         SteganoHide::getInstance().loadPicture(containerFilePath);
    } catch(SteganoException &ex) {
        wxString messageText = ex.what();
        ShowErrorDialog(_T("Es ist ein Fehler aufgetreten"), messageText);
    }

    startHidingButton->Enable(false);
    progressBar->SetValue(0);
    std::thread workThread([=]() mutable {
        try {
            std::ifstream hideFileStream(hideFilePath, std::fstream::binary);
            SteganoHide::getInstance().setOutputFilePath(outputFilePath);
            SteganoHide::getInstance().hideFile(hideFileStream, "notusedpassword");
            SteganoHide::getInstance().saveChangesToDisk();
            hideFileStream.close();
        }catch(SteganoException &ex) {
            wxString messageText = ex.what();
            ShowErrorDialog(_T("Error hiding file"), messageText);
        }
    });
    workThread.detach();
    std::thread prograssBarThread([&] () {
        while(progressBar->GetValue() < 100) {
            progressBar->SetValue(SteganoHide::getInstance().getDoneStateInPercent());
        }
        // we have to wait one second until user can repress the button because the ressources aren't freed properly íf we don't
        std::this_thread::sleep_for(std::chrono::seconds(1));
        startHidingButton->Enable(true);
    });
    prograssBarThread.detach();
}

void HidePage::OnOpenFileClicked(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, _("Eingabedatei wählen"), "", "", "Alle Dateien (*.*)|*.*", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
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
        hideFileInput->SetInsertionPointEnd();
    } else {
        containerFileInput->ChangeValue(userInputPath);
        containerFileInput->SetInsertionPointEnd();
    }
    userChosenFileStream.close();
}

void HidePage::OnSelectOutputFileClicked(wxCommandEvent &event) {
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
    outputFileInput->SetInsertionPointEnd();
    userChosenFileStream.close();
}

void HidePage::HideRadioChanged(wxCommandEvent& event) {
    if(event.GetId() == ID_HideTextRadio) {
        openHideFileDialogButton->Enable(false);
        textToHideCtrl->Enable(true);
    } else {
        openHideFileDialogButton->Enable(true);
        textToHideCtrl->Enable(false);
    }
}



// TODO: this is somehow not working
void HidePage::ShowErrorDialog(const wxString &title, const wxString &message) {
  wxMessageDialog *dial = new wxMessageDialog(this, message, title, wxICON_ERROR);
  dial->ShowModal();
}


