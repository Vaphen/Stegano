#ifndef EXPOSEPAGE_H
#define EXPOSEPAGE_H


#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/gbsizer.h>
#include <thread>

#include "SteganoExpose.h"
#include "ActionNotebookPage.h"

class ExposePage : public ActionNotebookPage
{
public:
    ExposePage() = delete;
    ExposePage(wxWindow*, int);
    virtual ~ExposePage();
protected:
private:
    wxBoxSizer *mainSizer;
    wxTextCtrl *inputFileCtrl;
    wxTextCtrl *outputFileCtrl;
    wxButton *startBtn;
    wxGauge *progressBar;

    // events
    void OnOpenInputDialogClicked(wxCommandEvent &);
    void OnOpenOutputDialogClicked(wxCommandEvent &);
    void OnStartClicked(wxCommandEvent &);

    // virtual functions from ActionNotebookPage
    void disableInputElements();
    void reenableElements();
    void startProgressbarThread();

    // additional functions
    void startExposeThread();

    wxDECLARE_EVENT_TABLE();
};

#endif // EXPOSEPAGE_H
