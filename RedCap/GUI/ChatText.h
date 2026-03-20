
class CChatText : public wxTextCtrl
{
public:

	    //MyTextCtrl(wxWindow *parent, wxWindowID id, const wxString &value,
     //          const wxPoint &pos, const wxSize &size, int style = 0)
     //   : wxTextCtrl(parent, id, value, pos, size, style)
	CChatText(wxSplitterWindow *parent, wxWindowID id, const wxString &value,
               const wxPoint &pos, const wxSize &size, int style = 0)
	        : wxTextCtrl(parent, id, value, pos, size, style)
    {
        
    }
	~CChatText(void);

	wxSplitterWindow		*BottomChatWindow;
	void					CheckKeys(wxKeyEvent& event);

	//	DECLARE_EVENT_TABLE()
};
