#ifndef HIDEPAGE_H
#define HIDEPAGE_H

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/gbsizer.h>
#include <thread>

#include "SteganoHide.h"


#define ID_OpenFile 1
#define ID_SelectOutputFile 2
#define ID_SelectContainerFile 3
#define ID_SelectHideFile 4
#define ID_HideFileRadio 5
#define ID_HideTextRadio 6
#define ID_Start 7

class HidePage : public wxNotebookPage
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
        wxTextCtrl *hideFileInput;
        wxBitmapButton *openContainerDialogButton;
        wxBitmapButton *openHideFileDialogButton;
        wxTextCtrl *textToHideCtrl;
        wxButton *startHidingButton;
        wxGauge *progressBar;

        // events
        void OnSelectOutputFileClicked(wxCommandEvent &event);
        void OnOpenFileClicked(wxCommandEvent& event);
        void HideRadioChanged(wxCommandEvent& event);
        void OnStart(wxCommandEvent &event);

        // initialization functions
        void addOutputFileArea();
        void addContainerFileArea();
        void addFileToHideArea();
        void addPhraseToHideArea();

        // additional functions
        void ShowErrorDialog(const wxString &, const wxString &);

        // declare event table
        wxDECLARE_EVENT_TABLE();
};

#endif // HIDEPAGE_H
