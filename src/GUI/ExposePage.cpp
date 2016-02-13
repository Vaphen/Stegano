#include "ExposePage.h"

BEGIN_EVENT_TABLE(ExposePage, ActionNotebookPage)
    EVT_BUTTON(ID_OpenInputDialog, ExposePage::OnOpenInputDialogClicked)
    EVT_BUTTON(ID_OpenOutputDialog, ExposePage::OnOpenOutputDialogClicked)
    EVT_BUTTON(ID_Start, ExposePage::OnStartClicked)
END_EVENT_TABLE()

ExposePage::ExposePage(wxWindow *parent, int id) :
    ActionNotebookPage(parent, id),
    mainSizer(new wxBoxSizer(wxVERTICAL)),
    inputFileCtrl(new wxTextCtrl(this, ID_InputCtrl, _T(""), wxDefaultPosition, wxSize(240, -1))),
    outputFileCtrl(new wxTextCtrl(this, ID_OutputCtrl, _T(""), wxDefaultPosition, wxSize(240, -1))),
    startBtn(new wxButton(this, ID_Start, "Extrahiere Daten")),
    progressBar(new wxGauge(this, wxID_ANY, 100))
{
    inputFileCtrl->Enable(false);
    outputFileCtrl->Enable(false);


    wxGridBagSizer *form = new wxGridBagSizer(20, 10);
    wxBitmap *openDialogButtonIcon = new wxBitmap("./res/folder.png", wxBITMAP_TYPE_PNG);

    wxStaticText *inputFileLabel = new wxStaticText(this, wxID_ANY, "Kontainer-Datei:");
    wxBitmapButton *openInputFileButton = new wxBitmapButton(this, ID_OpenInputDialog, *openDialogButtonIcon);

    wxStaticText *outputFileLabel = new wxStaticText(this, wxID_ANY, "Ausgabe-Datei:");
    wxBitmapButton *openOutputFileButton = new wxBitmapButton(this, ID_OpenOutputDialog, *openDialogButtonIcon);

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

void ExposePage::OnOpenInputDialogClicked(wxCommandEvent &event)
{
    wxFileDialog openFileDialog(this, _("Open XYZ file"), "", "", "PortableNetworkGraphics (*.png)|*.png", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    // user cancelled his action
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    std::string userInputPath = openFileDialog.GetPath().ToStdString();

    inputFileCtrl->ChangeValue(userInputPath);
    inputFileCtrl->SetInsertionPointEnd();
}

void ExposePage::OnOpenOutputDialogClicked(wxCommandEvent &event)
{
    wxFileDialog openFileDialog(this, _("Ausgabedatei wählen"), "", "", "Alle Dateien (*.*)|*.*", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    // user cancelled his action
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    std::string userInputPath = openFileDialog.GetPath().ToStdString();

    outputFileCtrl->ChangeValue(userInputPath);
    outputFileCtrl->SetInsertionPointEnd();
}


void ExposePage::OnStartClicked(wxCommandEvent &event)
{
    progressBar->SetValue(0);
    disableInputElements();
    startExposeThread();
    startProgressbarThread();
}

void ExposePage::disableInputElements()
{
    startBtn->Enable(false);
    startBtn->SetLabel("Extrahiere...");
}

void ExposePage::reenableElements()
{
    startBtn->SetLabel("Start!");
    startBtn->Enable(true);
}

void ExposePage::startProgressbarThread()
{
    std::thread progressBarThread([this]()
    {
        while(steganoRunning)
        {
            progressBar->SetValue(SteganoExpose::getInstance().getDoneStateInPercent());
        };
    });
    progressBarThread.detach();
}

void ExposePage::startExposeThread()
{
    steganoRunning = true;
    std::thread exposeThread([this]()
    {
        try
        {
            SteganoExpose::getInstance().loadPicture(inputFileCtrl->GetValue().ToStdString());
            std::stringstream exposedDataStream = SteganoExpose::getInstance().unhidePhrase("notusedpassword");
            std::ofstream outputFileStream(outputFileCtrl->GetValue().ToStdString(), std::fstream::binary);
            if(!outputFileStream.good())
            {
                throw SteganoException::OutputFileStreamNotGood();
            }
            outputFileStream << exposedDataStream.rdbuf();
            outputFileStream.close();
        }
        catch(const std::exception &ex)
        {
            sendThreadMsg(MessageData(_T("Ein Fehler ist aufgetreten."), wxString(ex.what())));
            steganoRunning = false;
            return;
        }
        sendThreadMsg(MessageData(_T("Extraktion erfolgreich!"), _T("Die Daten wurden erfolgreich extrahiert.")));
        steganoRunning = false;
    });
    exposeThread.detach();
}

