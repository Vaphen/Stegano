#ifndef HIDEPAGE_H
#define HIDEPAGE_H

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/gbsizer.h>
#include <wx/event.h>
#include <thread>
#include <cassert>
#include <utility>

#include "ActionNotebookPage.h"
#include "SteganoHide.h"
#include "Dialogs.h"

class HidePage : public ActionNotebookPage
{
public:
    HidePage() = delete;
    HidePage(wxWindow *, int);
    virtual ~HidePage();
protected:
private:
    // widgets
    wxGridBagSizer *form;
    wxBitmap *openDialogButtonIcon;

    wxTextCtrl *outputFileInput;
    wxTextCtrl *containerFileInput;
    wxRadioButton *hideFileRadio;
    wxTextCtrl *hideFileInput;
    wxBitmapButton *openContainerDialogButton;
    wxBitmapButton *openHideFileDialogButton;
    wxRadioButton *hideTextRadio;
    wxTextCtrl *textToHideCtrl;
    wxButton *startHidingButton;
    wxGauge *progressBar;


    // events
    void OnSelectOutputFileClicked(wxCommandEvent &event);
    void OnOpenHideFileClicked(wxCommandEvent& event);
    void OnOpenContainerFileClicked(wxCommandEvent& event);
    void HideRadioChanged(wxCommandEvent& event);
    void OnStart(wxCommandEvent &event);

    // initialization functions
    void addOutputFileArea();
    void addContainerFileArea();
    void addFileToHideArea();
    void addPhraseToHideArea();

    //void sendThreadMsg(const MessageData &);
    void disableInputElements();
    void reenableElements();
    void startProgressbarThread();

    void hideFileSelected(const std::string &);
    void hideTextSelected(const std::string &);

    // declare event table
    wxDECLARE_EVENT_TABLE();
};

#endif // HIDEPAGE_H
