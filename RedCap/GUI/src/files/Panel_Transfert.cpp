#include "general.h"

CTranfert::CTranfert(MyFrame *MainFrame, wxNotebook *parent, int xWin,int yWin)
{
	m_notebook = parent;
	panel_transfer = new wxPanel(m_notebook);

	/* on place les images en haut du panel */
#ifdef __WIN32__

	BitmapTree= new wxStaticBitmap(
								panel_transfer, -1,
								wxBitmap("./icons/BitmapTree.bmp", wxBITMAP_TYPE_BMP),
								wxPoint(0,0),
								wxSize(160, 24)
								);
	BitmapDegr= new wxStaticBitmap(
								panel_transfer, -1,
								wxBitmap("./icons/BitmapDegr.bmp", wxBITMAP_TYPE_BMP),
								wxPoint(160,0),
								wxSize( xWin -  365, 24)
								);
#else

	BitmapTree= new wxStaticBitmap(
								panel_transfer, -1,
								wxBitmap("./icons/file transfert_unix.bmp", wxBITMAP_TYPE_BMP),
								wxPoint(0,0),
								wxSize(310, 24)
								);
#endif

	/************** On place les explorateurs client et serveur *******************/

	SplitExplo = new wxSplitterWindow(panel_transfer, ID_SPLIT_EXPLO, wxPoint(0, 25), wxSize(xWin+8, yWin+160), 0);

	panel_server = new wxPanel(SplitExplo, -1, wxPoint(0, 0), wxDefaultSize, 0);
#ifdef __WIN32__
	ExploServer = new CExplorerServer(MainFrame, panel_server, 900, 800, yWin, "./icons/server_explo.bmp");
#else
	ExploServer = new CExplorerServer(MainFrame, panel_server, 900, 800, yWin, "./icons/img2.bmp");
#endif
	panel_client = new wxPanel(SplitExplo, -1, wxPoint(0, 0), wxDefaultSize, 0);

#ifdef __WIN32__
	ExploClient = new CExplorerClient(MainFrame, panel_client, 900, 800, yWin, "./icons/client_explo.bmp");
#else
	ExploClient = new CExplorerClient(MainFrame, panel_client, 900, 800, yWin, "./icons/img3.bmp");
#endif

	SplitExplo->SplitHorizontally(panel_server, panel_client);
	SplitExplo->SetMinimumPaneSize(100);
	m_notebook->AddPage(panel_transfer, wxT("Explorer"), FALSE, 1);
}

void MyPanel::OnDbleClickTransferServer(wxTreeEvent& event)
{
	wxMessageBox("tete");
}

void MyPanel::OnDbleClickTransferClient(wxTreeEvent& event)
{
	wxMessageBox("tete");
}

CTranfert::~CTranfert(void)
{
}
