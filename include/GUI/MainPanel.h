#ifndef MAINPANEL_H
#define MAINPANEL_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include "HidePage.h"
#include "ExposePage.h"

class MainPanel : public wxNotebook
{
    public:
        MainPanel(wxWindow *);
        virtual ~MainPanel();
    protected:
    private:
};

#endif // MAINPANEL_H
