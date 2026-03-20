#include "general.h"

class CChatText;

CPanelPublic::CPanelPublic(MyPanel *parent, int xWin,int yWin)
{
	m_panel = (MyPanel *)parent;

	panel_public = new wxPanel(m_panel->m_notebook);

#ifdef __WIN32__
	
	BitmapChat= new wxStaticBitmap(
								panel_public, -1,
								wxBitmap("./icons/BitmapChat.bmp", wxBITMAP_TYPE_BMP),
								wxPoint(0,0),
								wxSize(160, 24)
								);

	BitmapDegr= new wxStaticBitmap(
								panel_public, -1,
								wxBitmap("./icons/BitmapDegr.bmp", wxBITMAP_TYPE_BMP),
								wxPoint(160,0),
								wxSize( xWin -  365, 24)
								);
#else

	BitmapChat= new wxStaticBitmap(
								panel_public, -1,
								wxBitmap("./icons/publicchat_unix.bmp", wxBITMAP_TYPE_BMP),
								wxPoint(0,0),
								wxSize(310, 24)
								);

#endif

	splitter = new wxSplitterWindow(panel_public, ID_CHAT_SPLITTER, wxPoint(0, 24), wxDefaultSize, 0);
	TopChatWindow = new wxTextCtrl(splitter, -1, wxT(""), wxDefaultPosition,  wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);
	BottomChatWindow = new wxSplitterWindow(splitter, -1, wxPoint(0, 0), wxDefaultSize, 0);
	BottomChatWindow->SetBackgroundColour(*wxWHITE );
	ChatText = new wxTextCtrl(BottomChatWindow, ID_TEXTCTRL, wxT(""), 
					wxDefaultPosition, 
					wxDefaultSize, wxTE_MULTILINE);
		
	SendChatButton = new wxBitmapButton
								(
                                  BottomChatWindow, ID_SEND_PUBLIC_CHAT,
                                  wxBitmap("./icons/chat_button.bmp",wxBITMAP_TYPE_BMP),
                                  wxPoint(xWin - 88, yWin - 88),
								  wxDefaultSize,
								  wxBU_AUTODRAW
                                 );
	splitter->SplitHorizontally(TopChatWindow, BottomChatWindow);
	splitter->SetMinimumPaneSize( 60 );

	m_panel->m_notebook->AddPage(panel_public, _T("Chat Public"), FALSE, 0);
}



CPanelPublic::~CPanelPublic(void)
{
}
