
class CDownload: public wxPanel
{
public:
	CDownload(wxNotebook *m_notebook, int xWin, int yWin);
	~CDownload(void);

	wxListCtrl				*tree_list_download;
	wxPanel					*panel_download;
	wxStaticBitmap			*BitmapDownload;
	wxStaticBitmap			*BitmapDegr;
	wxBitmapButton 			*resume_file ;
	wxBitmapButton 			*pause_file;
	wxBitmapButton 			*erase_file;
	wxString				task_selected_file;
	wxString				task_selected_ref;
};
