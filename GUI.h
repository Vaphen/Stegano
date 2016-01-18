#ifndef GUI_H
#define GUI_H

#include <wx/frame.h>


class GUI: public wxFrame
{
	public:

		GUI();
		virtual ~GUI();




	protected:



	private:


        virtual void OnClose(wxCloseEvent&);
        void OnHello(wxCommandEvent& event);
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void createMenuBar();
        wxDECLARE_EVENT_TABLE();
};

#endif
