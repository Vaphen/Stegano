#ifndef GUI_H
#define GUI_H

#include <wx/frame.h>
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
#include <wx/dialog.h>

#include "SteganoHide.h"

class GUI: public wxFrame
{
	public:

		GUI();
		virtual ~GUI();




	protected:



	private:
        virtual void OnClose(wxCloseEvent&);
        void OnSelectOutputFileClicked(wxCommandEvent &event);
        void OnOpenFileClicked(wxCommandEvent& event);
        void HideRadioChanged(wxCommandEvent& event);
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnStart(wxCommandEvent &event);
        void createMenuBar();
        void ShowErrorDialog(const wxString &, const wxString &);
        wxDECLARE_EVENT_TABLE();

        // widgets
        wxTextCtrl *outputFileInput;
        wxTextCtrl *containerFileInput;
        wxTextCtrl *hideFileInput;
        wxBitmapButton *openContainerDialogButton;
        wxBitmapButton *openHideFileDialogButton;
        wxTextCtrl *textToHideCtrl;
};

#endif
