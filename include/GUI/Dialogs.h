#ifndef DIALOGS_H
#define DIALOGS_H

#include <wx/wx.h>
#include <utility>

typedef std::pair<wxString, wxString> MessageData;

class Dialogs
{
public:
    Dialogs(wxWindow *);
    virtual ~Dialogs();

    void ShowErrorDialog(const wxString &, const wxString &);
    void ShowInfoDialog(const wxString &, const wxString &);
    void ShowInfoDialog(const MessageData &);
protected:
private:
    wxWindow *parent;
};

#endif // DIALOGS_H
