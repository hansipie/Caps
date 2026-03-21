#include "general.h"

CPrivChat::CPrivChat(wxNotebook *parent, const wxString& nom, int xWin, int yWin)
{

	m_notebook = parent;
	name = nom;
	sous_panel = new wxPanel(m_notebook);

#ifdef __WIN32__
 
	BitmapPriv= new wxStaticBitmap(
									sous_panel, -1,
									wxBitmap("./icons/BitmapPriv.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(0,0),
									wxSize(160, 24)
									);

	BitmapDegr= new wxStaticBitmap(
									sous_panel, -1,
									wxBitmap("./icons/BitmapDegr.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(160,0),
									wxSize( xWin -  365, 24)
									);
#else

	BitmapPriv= new wxStaticBitmap(
									sous_panel, -1,
									wxBitmap("./icons/privatechat_unix.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(0,0),
									wxSize(310, 24)
									);
#endif


	splitter = new wxSplitterWindow(sous_panel, ID_CHAT_SPLITTER, wxPoint(0, 0), wxDefaultSize, 0);
	TopChatWindow = new wxTextCtrl(splitter, ID_TEXTCTRL, wxT(""), wxDefaultPosition,  wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);
	BottomChatWindow = new wxSplitterWindow(splitter, -1, wxDefaultPosition, wxDefaultSize, 0);
	BottomChatWindow->SetBackgroundColour(*wxWHITE );
	CloseButton = new wxBitmapButton
									(
									sous_panel, ID_CLOSE_CHAT,
									wxBitmap("./icons/close_button.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(xWin - 30, 1),
									wxSize(24, 24),
									wxBU_AUTODRAW
									);
    ChatText = new wxTextCtrl(BottomChatWindow, ID_TEXTCTRL, wxT(""), wxDefaultPosition, wxSize(xWin - 80, yWin + 2), wxTE_MULTILINE);
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
	m_notebook->AddPage(sous_panel, name, TRUE, 3);
}

CPrivChat::~CPrivChat(void)
{
}
