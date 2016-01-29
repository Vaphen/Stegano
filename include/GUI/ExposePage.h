#ifndef EXPOSEPAGE_H
#define EXPOSEPAGE_H


#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/gbsizer.h>
#include <thread>

#include "SteganoUnhide.h"

class ExposePage : public wxNotebookPage
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

        wxDECLARE_EVENT_TABLE();
};

#endif // EXPOSEPAGE_H
