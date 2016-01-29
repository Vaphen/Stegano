#include "ExposePage.h"

#define ID_START 1
#define ID_INPUT_CTRL 2
#define ID_OUTPUT_CTRL 3
#define ID_OPEN_INPUT_DIALOG 4
#define ID_OPEN_OUTPUT_DIALOG 5

BEGIN_EVENT_TABLE(ExposePage, wxNotebookPage)
    EVT_BUTTON(ID_OPEN_INPUT_DIALOG, ExposePage::OnOpenInputDialogClicked)
    EVT_BUTTON(ID_OPEN_OUTPUT_DIALOG, ExposePage::OnOpenOutputDialogClicked)
    EVT_BUTTON(ID_START, ExposePage::OnStartClicked)
END_EVENT_TABLE()

ExposePage::ExposePage(wxWindow *parent, int id) :
    wxNotebookPage(parent, id),
    mainSizer(new wxBoxSizer(wxVERTICAL)),
    inputFileCtrl(new wxTextCtrl(this, ID_INPUT_CTRL, _T(""), wxDefaultPosition, wxSize(240, -1))),
    outputFileCtrl(new wxTextCtrl(this, ID_OUTPUT_CTRL, _T(""), wxDefaultPosition, wxSize(240, -1))),
    startBtn(new wxButton(this, ID_START, "Extrahiere Daten")),
    progressBar(new wxGauge(this, wxID_ANY, 100))
{
    inputFileCtrl->Enable(false);
    outputFileCtrl->Enable(false);


    wxGridBagSizer *form = new wxGridBagSizer(20, 10);
    wxBitmap *openDialogButtonIcon = new wxBitmap("./res/folder.png", wxBITMAP_TYPE_PNG);

    wxStaticText *inputFileLabel = new wxStaticText(this, wxID_ANY, "Kontainer-Datei:");
    wxBitmapButton *openInputFileButton = new wxBitmapButton(this, ID_OPEN_INPUT_DIALOG, *openDialogButtonIcon);

    wxStaticText *outputFileLabel = new wxStaticText(this, wxID_ANY, "Ausgabe-Datei:");
    wxBitmapButton *openOutputFileButton = new wxBitmapButton(this, ID_OPEN_OUTPUT_DIALOG, *openDialogButtonIcon);

    form->Add(inputFileLabel, wxGBPosition(0, 0));
    form->Add(inputFileCtrl, wxGBPosition(0, 1), wxGBSpan(1, 1), wxEXPAND);
    form->Add(openInputFileButton, wxGBPosition(0, 2));

    form->Add(outputFileLabel, wxGBPosition(1, 0));
    form->Add(outputFileCtrl, wxGBPosition(1, 1), wxGBSpan(1, 1), wxEXPAND);
    form->Add(openOutputFileButton, wxGBPosition(1, 2));

    form->Add(startBtn, wxGBPosition(2, 0), wxGBSpan(1, 1), wxEXPAND);
    form->Add(progressBar, wxGBPosition(2, 1), wxGBSpan(1, 2), wxEXPAND);
    mainSizer->Add(form, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 30);

    this->SetSizer(mainSizer);
}

ExposePage::~ExposePage()
{
    //dtor
}

void ExposePage::OnOpenInputDialogClicked(wxCommandEvent &event) {
    wxFileDialog openFileDialog(this, _("Open XYZ file"), "", "", "PortableNetworkGraphics (*.png)|*.png", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    // user cancelled his action
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    std::string userInputPath = openFileDialog.GetPath().ToStdString();

    inputFileCtrl->ChangeValue(userInputPath);
    inputFileCtrl->SetInsertionPointEnd();
}

void ExposePage::OnOpenOutputDialogClicked(wxCommandEvent &event) {
    wxFileDialog openFileDialog(this, _("Ausgabedatei wählen"), "", "", "Alle Dateien (*.*)|*.*", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    // user cancelled his action
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    std::string userInputPath = openFileDialog.GetPath().ToStdString();

    outputFileCtrl->ChangeValue(userInputPath);
    outputFileCtrl->SetInsertionPointEnd();
}


void ExposePage::OnStartClicked(wxCommandEvent &event) {
    progressBar->SetValue(0);
    startBtn->Enable(false);
    std::thread exposeThread([this]() {
        SteganoUnhide::getInstance().loadPicture(inputFileCtrl->GetValue().ToStdString());
        std::stringstream exposedDataStream = SteganoUnhide::getInstance().unhidePhrase("notusedpassword");
        std::ofstream outputFileStream(outputFileCtrl->GetValue().ToStdString(), std::fstream::binary);
        if(!outputFileStream.good()) {
                // showErrorDialog();
            return;
        }
        outputFileStream << exposedDataStream.rdbuf();
        outputFileStream.close();
    });
    exposeThread.detach();

    std::thread progressBarThread([this]() {
        while(progressBar->GetValue() < 100) {
            progressBar->SetValue(SteganoUnhide::getInstance().getDoneStateInPercent());
        };
        std::this_thread::sleep_for(std::chrono::seconds(1));
        startBtn->Enable(true);
    });
    progressBarThread.detach();

}

