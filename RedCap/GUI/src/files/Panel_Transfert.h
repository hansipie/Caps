class CExplorer;

class CTranfert: public wxPanel
{
public:
	CTranfert(MyFrame *MainFrame, wxNotebook *m_notebook, int xWin,int yWin);
	~CTranfert(void);
	void OnFileSashChange();

	wxPanel					*panel_transfer;
	CExplorerServer			*ExploServer;
	CExplorerClient			*ExploClient;
	wxPanel					*panel_server;
	wxPanel					*panel_client;
	wxStaticBitmap			*BitmapTree;
	wxStaticBitmap			*BitmapDegr;
	wxSplitterWindow		*SplitExplo;
	wxNotebook				*m_notebook;
};
