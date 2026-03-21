#include "general.h"

CNewsGroup::CNewsGroup(wxNotebook *parent, int xWin,int yWin)
{
	m_notebook = parent;
	panel_news = new wxPanel(m_notebook);

	/* on place les images en haut du panel */
	BitmapNews= new wxStaticBitmap(
								panel_news, -1,
								wxBitmap("./icons/BitmapTree.bmp", wxBITMAP_TYPE_BMP),
								wxPoint(0,0),
								wxSize(160, 24)
								);

	BitmapDegr= new wxStaticBitmap(
								panel_news, -1,
								wxBitmap("./icons/BitmapDegr.bmp", wxBITMAP_TYPE_BMP),
								wxPoint(160,0),
								wxSize( xWin -  365, 24)
								);
	
	
	wxSplitterWindow 	*SplitNews = new wxSplitterWindow(panel_news, -1, wxPoint(0, 24), wxDefaultSize, 0);
	wxTextCtrl			*TopChatWindow = new wxTextCtrl(SplitNews, -1, wxT(""), wxDefaultPosition,  wxSize(150, 150), wxTE_MULTILINE | wxTE_READONLY );
	wxSplitterWindow	*BottomChatWindow = new wxSplitterWindow(SplitNews, -1, wxDefaultPosition, wxSize(150, 150), 0);

	SplitNews->SplitHorizontally(TopChatWindow, BottomChatWindow);
	SplitNews->SetSashPosition(150);
	m_notebook->AddPage(panel_news, wxT("NewsGroup"), FALSE, 1);
}

CNewsGroup::~CNewsGroup(void)
{
}
