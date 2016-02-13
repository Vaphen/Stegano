#ifndef ACTIONNOTEBOOKPAGE_H
#define ACTIONNOTEBOOKPAGE_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include "Dialogs.h"

typedef std::pair<wxString, wxString> MessageData;

// Event-IDS for ExposePage and HidePage
enum EVENT_IDS
{
    ID_Start,
    ID_InputCtrl,
    ID_OutputCtrl,
    ID_OpenInputDialog,
    ID_OpenOutputDialog,
    ID_OpenFile,
    ID_SelectOutputFile,
    ID_SelectContainerFile,
    ID_SelectHideFile,
    ID_HideFileRadio,
    ID_HideTextRadio,
    ID_ShowThreadMsg
};


class ActionNotebookPage : public wxNotebookPage
{
public:
    ActionNotebookPage() = delete;
    ActionNotebookPage(wxWindow *parent, int widgetID);
    virtual ~ActionNotebookPage();

protected:
    static bool steganoRunning;

    void sendThreadMsg(const MessageData &);
    virtual void disableInputElements() = 0;
    virtual void reenableElements() = 0;
    virtual void startProgressbarThread() = 0;

    // events
    void showThreadMessage(wxThreadEvent &event);

    // dialog builder
    Dialogs dialogBuilder;

    // declare event table
    wxDECLARE_EVENT_TABLE();
private:
};

#endif // ACTIONNOTEBOOKPAGE_H
