#include "ImagePanel.h"

BEGIN_EVENT_TABLE(ImagePanel, wxPanel)
    EVT_PAINT(ImagePanel::paintEvent)
END_EVENT_TABLE()

ImagePanel::ImagePanel(wxFrame* parent, wxString file, wxBitmapType format) : wxPanel(parent)
{
    // load the file... ideally add a check to see if loading was successful
    image.LoadFile(file, format);
    this->SetMinSize(wxSize(image.GetWidth(), image.GetHeight()));

}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void ImagePanel::paintEvent(wxPaintEvent & evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void ImagePanel::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void ImagePanel::render(wxDC&  dc)
{
    dc.DrawBitmap( image, 0, 0, false );
}
