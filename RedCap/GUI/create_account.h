enum
{
	ID_CREATED_ACCOUNT
};

class CCreateAccount: public wxDialog
{
public:
	CCreateAccount(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos);
	~CCreateAccount(void);
	void OnConfirmNewAccount(wxCommandEvent &event);

	 MyFrame	*MainFrame;	
	 CAdmin		*Admin;
	 wxTextCtrl *name_field;
	 wxTextCtrl *login_field;
	 wxTextCtrl *password_field;
	 	 wxString CreateAccountCmd;
 
private:
	DECLARE_EVENT_TABLE();
};
