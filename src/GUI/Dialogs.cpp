#include "Dialogs.h"

Dialogs::Dialogs(wxWindow *parent) :
    parent(parent)
{ }

Dialogs::~Dialogs()
{ }


void Dialogs::ShowErrorDialog(const wxString &title, const wxString &message)
{
    wxMessageDialog *dial = new wxMessageDialog(parent, message, title, wxICON_ERROR);
    dial->ShowModal();
}

void Dialogs::ShowInfoDialog(const wxString &title, const wxString &message)
{
    wxMessageDialog *dial = new wxMessageDialog(parent, message, title, wxICON_INFORMATION);
    dial->ShowModal();
}

void Dialogs::ShowInfoDialog(const MessageData &data)
{
    wxMessageDialog *dial = new wxMessageDialog(parent, data.second, data.first, wxICON_INFORMATION);
    dial->ShowModal();
}
