
class CExplorerServer: public wxPanel
{
public:
	CExplorerServer(MyFrame *MainFrame, wxPanel *parent, int height, int xWin, int Ywin, wxChar *bitmap_name);
	~CExplorerServer(void);


	wxPanel					*left_page;
	wxPanel					*right_page;
	wxSplitterWindow		*explo_window;
//	wxTreeCtrl				*tree_transfer;
	wxListCtrl				*tree_list;
	wxStaticBitmap			*bitmap_left;
	wxStaticBitmap			*bitmap_left2;
	wxImageList				*m_imageTreeCtrl;

	wxBitmapButton 			*delete_button ;
	wxBitmapButton 			*create_folder_button;
	wxBitmapButton 			*reload_button;
	wxBitmapButton 			*download_button;
	wxBitmapButton 			*upload_button;
	wxBitmapButton 			*uplevel_button;
	wxBitmapButton 			*preview_button;
	wxBitmapButton 			*info_button;
	wxTextCtrl				*PathBar;
};
