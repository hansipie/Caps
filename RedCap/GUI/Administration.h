enum
{
	ID_LIST_ACCOUNT,
	BUTTON_SAVE,
	ID_CHECKBOX,
	ID_TEXT,
	ID_DWNL_FILE,
	ID_ADD_ACCOUNT_BUTTON,
	ID_SUPPR_ACCOUNT_BUTTON,
	ID_EMPTY_SUPPR = 242424
};



class CCreateAccount;
class CConfirmBox;

class CAdmin: public wxDialog
{
public:
	CAdmin(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos);
	~CAdmin(void);
	void OnCreateNewAccount(wxCommandEvent &event);
	void OnEraseAccount(wxCommandEvent &event);
	void OnSelectedAccount(wxCommandEvent& command);
	void WasModified(wxCommandEvent& command);
	void save_privileges(wxCommandEvent& command);
	void FillAcountList();
	 MyFrame				*MainFrame;

	CCreateAccount			*m_create_account;
	CConfirmBox				*m_ConfirmBox;
	wxListCtrl				*list_account;
	vector<wxString>		account_list;
	wxPanel					*panel_FilesRights;
	wxPanel					*panel_ChatRights;
	wxPanel					*panel_UserRights;
	wxPanel					*panel_MiscellaneousRights;
	bool					save_flag;
	wxString				prec_account;
		wxString pass;
	wxString NewAccessPrivileges;


	wxBitmapButton			*add_account_button;
	wxBitmapButton			*suppr_account_button;

	wxNotebook				*rights_notebook;
	wxStaticBitmap			*BitmapUser;

	wxTextCtrl				*password;
	wxTextCtrl				*login;
	wxTextCtrl				*UserName;

	wxCheckBox				*deletefile; //
	wxCheckBox				*uploadfile; //
	wxCheckBox				*downloadfile; //
	wxCheckBox				*renamefile; //
	wxCheckBox				*movefile; //
	wxCheckBox 				*getfilecomment; //

	wxCheckBox 				*createfolder;//
	wxCheckBox 				*deletefolder; //
	wxCheckBox 				*renamefolder;//
	wxCheckBox 				*movefolder; //
	wxCheckBox 				*setfoldercomment; //

	wxCheckBox 				*makealias;//
	wxCheckBox 				*viewdropboxes;//

	wxCheckBox 				*readchat;//
	wxCheckBox 				*sendchat;//
	wxCheckBox 				*openchat;//
	wxCheckBox 				*closechat;//
	wxCheckBox 				*sendprivatemessage;//
	wxCheckBox 				*broadcast;//

	wxCheckBox 				*showinlist;//
	wxCheckBox 				*createuser;//
	wxCheckBox 				*deleteuser;//
	wxCheckBox 				*openuser;//
	wxCheckBox 				*modifyuser;//
	wxCheckBox 				*changeownpassword;//
	
	wxCheckBox 				*disconnectuser;
	wxCheckBox 				*cannotbedisconnect;
	wxCheckBox 				*getclientinfo;
	
	wxCheckBox 				*uploadanywhere; //
	wxCheckBox 				*anyname;//
	wxCheckBox 				*noagreement;//
	wxCheckBox 				*DisconnectUser;//
	wxCheckBox				*GetClientInfo;//
	
	wxCheckBox 				*newsreadarticle;
	wxCheckBox 				*newspostarticle;
	wxCheckBox 				*newsdeletearticle;
	wxCheckBox 				*newscreatecategory;
	wxCheckBox 				*newsdeletecategory;
	wxCheckBox 				*newscreatefolder;
	wxCheckBox 				*newsdeletefolder;

	private:
	DECLARE_EVENT_TABLE();
};
