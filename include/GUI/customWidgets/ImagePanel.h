#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#include <wx/wx.h>
#include <wx/sizer.h>

/**
 * \brief This class came from the official wxWidgets webpage.
 */
class ImagePanel : public wxPanel
{
    wxBitmap image;

public:
    ImagePanel(wxFrame*, wxString, wxBitmapType);
    //virtual ~ImagePanel();

    void paintEvent(wxPaintEvent & evt);
    void paintNow();

    void render(wxDC& dc);

    DECLARE_EVENT_TABLE()
};

#endif // IMAGEPANEL_H
