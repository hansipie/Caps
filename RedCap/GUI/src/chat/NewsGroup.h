
class CNewsGroup
{
public:
	CNewsGroup(wxNotebook *m_notebook, int xWin,int yWin);
	~CNewsGroup(void);
	wxPanel					*panel_news;
	wxStaticBitmap			*BitmapNews;
	wxStaticBitmap			*BitmapDegr;
	wxNotebook *m_notebook;
};
