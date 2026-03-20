class CPrivChat: public wxPanel
{
public:
	CPrivChat(wxNotebook *parent, const wxChar *nom, int xWin, int yWin);
	~CPrivChat(void);
	const wxChar			*name;
	int						id_user;
	wxNotebook				*m_notebook;
	wxPanel					*sous_panel;
	wxSplitterWindow		*splitter;
	wxTextCtrl				*TopChatWindow;
	wxSplitterWindow		*BottomChatWindow;
    wxTextCtrl				*ChatText;
    wxBitmapButton			*SendChatButton;
	wxBitmapButton			*CloseButton;
	wxBoxSizer				*sizer0;
	int						y_sash;
	wxStaticBitmap			*BitmapPriv;
	wxStaticBitmap			*BitmapDegr;

	//void OnSize(wxSizeEvent& WXUNUSED(event));
	 DECLARE_EVENT_TABLE()
};
