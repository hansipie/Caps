
class CChatText;

class CPanelPublic: public wxPanel
{
public:
	CPanelPublic(MyPanel *parent, int xWin,int yWin);
	~CPanelPublic(void);
	
	wxPanel					*panel_public;
	MyPanel					*m_panel;

	wxSplitterWindow		*splitter;
	wxSplitterWindow		*BottomSplitter;
	wxBitmapButton			*SendChatButton;
	wxSplitterWindow		*BottomChatWindow;
	wxTextCtrl				*TopChatWindow; 
	wxTextCtrl				*ChatText;
	wxBoxSizer				*sizer0;
	wxStaticBitmap			*BitmapChat;
	wxStaticBitmap			*BitmapDegr;
	int						y_sash;
};
