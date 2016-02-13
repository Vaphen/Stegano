#include "ActionNotebookPage.h"

BEGIN_EVENT_TABLE(ActionNotebookPage, wxNotebookPage)
    EVT_THREAD(ID_ShowThreadMsg, ActionNotebookPage::showThreadMessage)
END_EVENT_TABLE()

// true while hiding is in progress, afterwards false. used to communicate between hiding- and progressbarthread
bool ActionNotebookPage::steganoRunning = false;

ActionNotebookPage::ActionNotebookPage(wxWindow *parent, int widgetID) :
    wxNotebookPage(parent, widgetID),
    dialogBuilder(this)
{

}

ActionNotebookPage::~ActionNotebookPage() { }

void ActionNotebookPage::sendThreadMsg(const MessageData &data)
{
    wxThreadEvent *evt = new wxThreadEvent(wxEVT_THREAD, ID_ShowThreadMsg);
    evt->SetPayload(data);
    QueueEvent(evt);
}

void ActionNotebookPage::showThreadMessage(wxThreadEvent &event)
{
    MessageData eventData = event.GetPayload<MessageData>();
    dialogBuilder.ShowInfoDialog(eventData.first, eventData.second);
    reenableElements();
}

