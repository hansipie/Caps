#include "general.h"

//CChatText::CChatText(wxSplitterWindow *parent, wxWindowID id, const wxString &value,
//               const wxPoint &pos, const wxSize &size, int style)
//{
//	/*BottomChatWindow = parent;
//	
//	Chatter = new wxTextCtrl(BottomChatWindow, ID_TEXTCTRL, wxT(""), 
//					wxPoint(0, 0), 
//					wxSize(100, 100), wxTE_MULTILINE);*/
//
//}

void CChatText::CheckKeys(wxKeyEvent& event)
{
	long keycode = event.GetKeyCode();

	if (keycode == WXK_RETURN)
	{
		wxMessageBox("toot");
	}
}

CChatText::~CChatText(void)
{
}
