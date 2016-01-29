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
#include <wx/panel.h>
#include <thread>

#include "SteganoHide.h"
#include "MainPanel.h"
#include "customWidgets/ImagePanel.h"

class GUI: public wxFrame
{
	public:

		GUI();
		virtual ~GUI();

	protected:

	private:
        virtual void OnClose(wxCloseEvent&);
        void createMenuBar();
        void initializeWindow();

        // events
        void OnAbout(wxCommandEvent&);
        void OnExit(wxCommandEvent&);

        wxDECLARE_EVENT_TABLE();
};

#endif
