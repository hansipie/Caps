#include "general.h"

CAdmin::CAdmin(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos)
:wxDialog(parent, id, title, pos, wxSize(520, 500))
{
	MainFrame = (MyFrame *)parent;
    parent = this;
	save_flag = 0;
	wxBitmap AdminBitmaps[2];
	AdminBitmaps[0] = wxBitmap("./icons/add_useraccount.bmp",wxBITMAP_TYPE_BMP);
	AdminBitmaps[1] = wxBitmap("./icons/suppr_account.bmp",wxBITMAP_TYPE_BMP);
	
	wxMask *mask0 = new wxMask(AdminBitmaps[0], *wxWHITE);
    AdminBitmaps[0].SetMask(mask0);
	wxMask *mask1 = new wxMask(AdminBitmaps[1], *wxWHITE);
    AdminBitmaps[1].SetMask(mask1);
	
	add_account_button = new wxBitmapButton(parent, ID_ADD_ACCOUNT_BUTTON,
								AdminBitmaps[0],
								wxPoint(5, 8),
								wxSize(30, 30));
	suppr_account_button = new wxBitmapButton(parent, ID_SUPPR_ACCOUNT_BUTTON, 
								AdminBitmaps[1], 
								wxPoint(220, 8),
								wxSize(30, 30));

//------------------------------------------------------------------------------------------------------------

	BitmapUser = new wxStaticBitmap(
									this, -1,
									wxBitmap("./icons/Accounts.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(5, 40),
									wxSize(230, 24)
									);

	list_account = new wxListCtrl(parent, 
										ID_LIST_ACCOUNT,
										wxPoint(5, 65), wxSize(230, 345), 
										wxSUNKEN_BORDER | /*wxLC_SMALL_ICON |*/ wxLC_LIST);

	list_account->InsertColumn( 0, wxT("Login"), wxLIST_FORMAT_LEFT, 110 );



//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

	wxStaticBox *item1;
	item1 = new wxStaticBox( parent, -1, wxT("User Account Information"), wxPoint(240, 40), wxSize(265, 130));
    
    wxStaticText *item3;
	item3 = new wxStaticText( parent, ID_TEXT, wxT("Name :"), wxPoint(250, 70), wxDefaultSize, 0 );
	UserName = new wxTextCtrl( parent, ID_TEXTCTRL, wxT(""), wxPoint(360, 70), wxSize(130,-1), 0 );

    wxStaticText *item5;
	item5 = new wxStaticText( parent, ID_TEXT, wxT("Login :"), wxPoint(250, 100), wxDefaultSize, 0 );
    login = new wxTextCtrl( parent, ID_TEXTCTRL, wxT(""), wxPoint(360, 100), wxSize(130,-1), 0 );

   wxStaticText *item7;
   item7 = new wxStaticText( parent, ID_TEXT, wxT("Password :"), wxPoint(250, 130), wxDefaultSize, 0 );
   password = new wxTextCtrl( parent, ID_TEXTCTRL, wxT(""), wxPoint(360, 130), wxSize(130,-1),wxTE_PASSWORD );

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

	rights_notebook = new wxNotebook(parent, ID_NOTEBOOK, wxPoint(240, 175),wxSize(270, 235), 0);		
	
	panel_FilesRights = new wxPanel(rights_notebook, -1);

	
	downloadfile = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can download Files"), 
					wxPoint(5, 35), wxDefaultSize, 0 );
	uploadfile = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can upload Files"), 
					wxPoint(5, 55), wxDefaultSize, 0 );
	uploadanywhere = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can upload Anywhere"), 
					wxPoint(5, 75), wxDefaultSize, 0 );
	deletefile = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can delete Files"), 
					wxPoint(5, 95), wxDefaultSize, 0 );	
	renamefile = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can rename Files"), 
					wxPoint(5, 115), wxDefaultSize, 0 );
	movefile = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can move Files"), 
					wxPoint(5, 135), wxDefaultSize, 0 );
	getfilecomment = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can comment Files"), 
					wxPoint(5, 155), wxDefaultSize, 0 );

	createfolder = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can create Folders"), 
					wxPoint(135, 35), wxDefaultSize, 0 );
	renamefolder = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can rename Folders"), 
					wxPoint(135, 55), wxDefaultSize, 0 );
	deletefolder = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can delete Folders"), 
					wxPoint(135, 75), wxDefaultSize, 0 );
	movefolder = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can move Folders"), 
					wxPoint(135, 95), wxDefaultSize, 0 );
	setfoldercomment = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can comment Folders"), 
					wxPoint(135, 115), wxDefaultSize, 0 );
	makealias = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can make Aliases"), 
					wxPoint(135, 135), wxDefaultSize, 0 );
	viewdropboxes = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can view Drop Boxes"), 
					wxPoint(135, 155), wxDefaultSize, 0 );
	rights_notebook->AddPage(panel_FilesRights, wxT("Files"));
	
	panel_UserRights = new wxPanel(rights_notebook, -1);
	createuser = new wxCheckBox( panel_UserRights, ID_DWNL_FILE, wxT("Can cresate Accounts"), 
					wxPoint(5, 35), wxDefaultSize, 0 );
	modifyuser = new wxCheckBox( panel_UserRights, ID_DWNL_FILE, wxT("Can modify Accounts"), 
					wxPoint(5, 55), wxDefaultSize, 0 );
	deleteuser = new wxCheckBox( panel_UserRights, ID_DWNL_FILE, wxT("Can delete Accounts"), 
					wxPoint(5, 75), wxDefaultSize, 0 );
	showinlist = new wxCheckBox( panel_UserRights, ID_DWNL_FILE, wxT("Can read Accounts"), 
					wxPoint(5, 95), wxDefaultSize, 0 );	
	openuser = new wxCheckBox( panel_UserRights, ID_DWNL_FILE, wxT("Can get User Info"), 
					wxPoint(5, 115), wxDefaultSize, 0 );	
	modifyuser = new wxCheckBox( panel_UserRights, ID_DWNL_FILE, wxT("Can modify Accounts"), 
					wxPoint(5, 135), wxDefaultSize, 0 );
	changeownpassword = new wxCheckBox( panel_UserRights, ID_DWNL_FILE, wxT("Can change own password"), 
					wxPoint(5, 155), wxDefaultSize, 0 );
	rights_notebook->AddPage(panel_UserRights, wxT("User"));
	
	panel_ChatRights = new wxPanel(rights_notebook, -1);	
	readchat = new wxCheckBox( panel_ChatRights, ID_DWNL_FILE, wxT("Can read Chat"), 
					wxPoint(5, 35), wxDefaultSize, 0 );
	sendchat = new wxCheckBox( panel_ChatRights, ID_DWNL_FILE, wxT("Can send Accounts"), 
					wxPoint(5, 55), wxDefaultSize, 0 );
	openchat = new wxCheckBox( panel_ChatRights, ID_DWNL_FILE, wxT("Can initiate Private Chat"), 
					wxPoint(5, 75), wxDefaultSize, 0 );
	closechat = new wxCheckBox( panel_ChatRights, ID_DWNL_FILE, wxT("Can close Chat"), 
					wxPoint(5, 95), wxDefaultSize, 0 );	
	sendprivatemessage = new wxCheckBox( panel_ChatRights, ID_DWNL_FILE, wxT("Can send Messages"), 
					wxPoint(5, 115), wxDefaultSize, 0 );	
	broadcast = new wxCheckBox( panel_ChatRights, ID_DWNL_FILE, wxT("Can Broadcast"), 
					wxPoint(5, 135), wxDefaultSize, 0 );
	rights_notebook->AddPage(panel_ChatRights, wxT("Chat"));
	panel_MiscellaneousRights = new wxPanel(rights_notebook, -1);
	anyname = new wxCheckBox( panel_MiscellaneousRights, ID_DWNL_FILE, wxT("Can use any name"), 
					wxPoint(5, 35), wxDefaultSize, 0 );
	noagreement = new wxCheckBox( panel_MiscellaneousRights, ID_DWNL_FILE, wxT("Don't show Agreement"), 
					wxPoint(5, 55), wxDefaultSize, 0 );
	DisconnectUser = new wxCheckBox( panel_MiscellaneousRights, ID_DWNL_FILE, wxT("Can Disconnect User"), 
					wxPoint(5, 75), wxDefaultSize, 0 );
	GetClientInfo = new wxCheckBox( panel_MiscellaneousRights, ID_DWNL_FILE, wxT("Get Client Info"), 
					wxPoint(5, 95), wxDefaultSize, 0 );
	rights_notebook->AddPage(panel_MiscellaneousRights, wxT("Miscellaneous"));
	
	wxButton *ok_opt_button;
	ok_opt_button = new wxButton( this, BUTTON_SAVE, wxT("Save"), wxPoint(345, 425), wxDefaultSize,0);
	wxButton *cancel_opt_button;
	cancel_opt_button = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxPoint(430, 425), wxDefaultSize,0);
	//MainFrame->output_message("get_login_list");
}


CAdmin::~CAdmin(void)
{
}

void CAdmin::FillAcountList()
{
	list_account->ClearAll();
	for ( vector<wxString>::iterator i = account_list.begin(); i != account_list.end(); ++i )
	{
		list_account->InsertItem(list_account->GetItemCount(),*i);
	}	
}
void CAdmin::OnCreateNewAccount(wxCommandEvent& command)
{

	m_create_account = new CCreateAccount(this, -1, wxT("New User Account"), wxDefaultPosition);
  	m_create_account->CentreOnParent();
  	m_create_account->ShowModal();
	//wxMessageBox("CreateAccountCmd : " + m_create_account->CreateAccountCmd);
	if (wxStrcmp(m_create_account->CreateAccountCmd, ""))
	{
		MainFrame->output_message(m_create_account->CreateAccountCmd);
		MainFrame->output_message("get_login_list");
	}

}




void CAdmin::OnEraseAccount(wxCommandEvent& command)
{
	wxString account;
	wxString suppr_message;
	int suppr;

	long item = list_account->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (item != -1)
		account = list_account->GetItemText(item);
	suppr_message = "Are you sure you want to delete the user \"" + account + "\" ?";

#ifdef __WIN32__

	if (IsEmpty(account))
	{
		wxMessageDialog MsgDlg_erreur(this, "You must select a User Account before deleting", "Save Administration Changes", wxOK | wxICON_EXCLAMATION);
		suppr = MsgDlg_erreur.ShowModal();
	}
	else
	{
		wxMessageDialog MsgDlg(this, suppr_message, "Save Administration Changes", wxYES_DEFAULT | wxNO | wxICON_EXCLAMATION);
		suppr = MsgDlg.ShowModal();
		if (suppr == wxID_YES)
		  {
		   // wxMessageBox("tu es sur le point de supprimer !!!");
		    MainFrame->output_message("delete_user " + account);
		    MainFrame->output_message("get_login_list");
		  }

	}
#else

   wxMessageBox("Vous etes sur le point de supprimer ce compte !!!");
   MainFrame->output_message("delete_user " + account);
  MainFrame->output_message("get_login_list");

#endif

}

void CAdmin::OnSelectedAccount(wxCommandEvent& command)
{	
	wxString account;

	int save;
	wxString save_message;
	

	//MainFrame->IntToBox(save_flag);
	long item = list_account->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (item != -1)
		account = list_account->GetItemText(item);

	
	if (save_flag == 1)
	{
		save_flag = 0;
		save_message = "Would you like to save changes on User \"" + prec_account + "\" ?";
		wxMessageDialog MsgDlg(this, save_message, "Save Adminstration Changes", wxYES_DEFAULT |wxNO | wxICON_EXCLAMATION);
		save = MsgDlg.ShowModal();
		if (save == wxID_YES)
		{
			pass = password->GetValue();
			if (IsEmpty(pass))
				pass = " ";
			NewAccessPrivileges = "set_user_edit \"" + login->GetValue() + "\" \"" + pass + "\" \"" + UserName->GetValue() + "\"";
			NewAccessPrivileges = NewAccessPrivileges + " \"" 
				+ ((deletefile->GetValue())?"1":"0")
				+ ((uploadfile->GetValue())?"1":"0")
				+ ((downloadfile->GetValue())?"1":"0")
				+ ((renamefile->GetValue())?"1":"0")
				+ ((movefile->GetValue())?"1":"0")
				+ ((createfolder->GetValue())?"1":"0")
				+ ((deletefolder->GetValue())?"1":"0")
				+ ((renamefolder->GetValue())?"1":"0")
				+ ((movefolder->GetValue())?"1":"0")
				+ ((readchat->GetValue())?"1":"0")
				+ ((sendchat->GetValue())?"1":"0")
				+ ((openchat->GetValue())?"1":"0")
				+ ((closechat->GetValue())?"1":"0")
				+ ((showinlist->GetValue())?"1":"0")
				+ ((createuser->GetValue())?"1":"0")
				+ ((deleteuser->GetValue())?"1":"0")
				+ ((openuser->GetValue())?"1":"0")
				+ ((modifyuser->GetValue())?"1":"0")
				+ ((changeownpassword->GetValue())?"1":"0")
				+ ((sendprivatemessage->GetValue())?"1":"0")
				+ "00"
				+ ((DisconnectUser->GetValue())?"1":"0")
				+ "0"
				//+ ((cannotbedisconnected->GetValue())?"1":"0")
				+ ((GetClientInfo->GetValue())?"1":"0")
				+ ((uploadanywhere->GetValue())?"1":"0")
				+ ((anyname->GetValue())?"1":"0")
				+ ((noagreement->GetValue())?"1":"0")
				+ ((getfilecomment->GetValue())?"1":"0")
				+ ((setfoldercomment->GetValue())?"1":"0")
				+ ((viewdropboxes->GetValue())?"1":"0")
				+ ((makealias->GetValue())?"1":"0")
				+ ((broadcast->GetValue())?"1":"0")
				+ "00000"
				//+ ((uploadfolder->GetValue())?"1":"0")
				//+ ((makealias->GetValue())?"1":"0")
				+ "00000000000000000000000000"
				+ "\""; 
			//wxMessageBox(NewAccessPrivileges);
			MainFrame->output_message(NewAccessPrivileges);
		}
		MainFrame->output_message("get_user " + account);
	}
	else 
		MainFrame->output_message("get_user " + account);
//	wxMessageBox("get_user \"" + account + "\"");
		prec_account = account;
}

void CAdmin::save_privileges(wxCommandEvent& command)
{
	pass = password->GetValue();
			if (IsEmpty(pass))
				pass = " ";
			NewAccessPrivileges = "set_user_edit \"" + login->GetValue() + "\" \"" + pass + "\" \"" + UserName->GetValue() + "\"";
			NewAccessPrivileges = NewAccessPrivileges + " \"" 
				+ ((deletefile->GetValue())?"1":"0")
				+ ((uploadfile->GetValue())?"1":"0")
				+ ((downloadfile->GetValue())?"1":"0")
				+ ((renamefile->GetValue())?"1":"0")
				+ ((movefile->GetValue())?"1":"0")
				+ ((createfolder->GetValue())?"1":"0")
				+ ((deletefolder->GetValue())?"1":"0")
				+ ((renamefolder->GetValue())?"1":"0")
				+ ((movefolder->GetValue())?"1":"0")
				+ ((readchat->GetValue())?"1":"0")
				+ ((sendchat->GetValue())?"1":"0")
				+ ((openchat->GetValue())?"1":"0")
				+ ((closechat->GetValue())?"1":"0")
				+ ((showinlist->GetValue())?"1":"0")
				+ ((createuser->GetValue())?"1":"0")
				+ ((deleteuser->GetValue())?"1":"0")
				+ ((openuser->GetValue())?"1":"0")
				+ ((modifyuser->GetValue())?"1":"0")
				+ ((changeownpassword->GetValue())?"1":"0")
				+ ((sendprivatemessage->GetValue())?"1":"0")
				+ "00"
				+ ((DisconnectUser->GetValue())?"1":"0")
				+ "0"
				//+ ((cannotbedisconnected->GetValue())?"1":"0")
				+ ((GetClientInfo->GetValue())?"1":"0")
				+ ((uploadanywhere->GetValue())?"1":"0")
				+ ((anyname->GetValue())?"1":"0")
				+ ((noagreement->GetValue())?"1":"0")
				+ ((getfilecomment->GetValue())?"1":"0")
				+ ((setfoldercomment->GetValue())?"1":"0")
				+ ((viewdropboxes->GetValue())?"1":"0")
				+ ((makealias->GetValue())?"1":"0")
				+ ((broadcast->GetValue())?"1":"0")
				+ "00000"
				//+ ((uploadfolder->GetValue())?"1":"0")
				//+ ((makealias->GetValue())?"1":"0")
				+ "00000000000000000000000000"
				+ "\""; 
			//wxMessageBox(NewAccessPrivileges);
			MainFrame->output_message(NewAccessPrivileges);
			save_flag = 0;
}

void CAdmin::WasModified(wxCommandEvent& command)
{	
	save_flag = 1;
	//wxMessageBox("checkbox Modified");
}

BEGIN_EVENT_TABLE(CAdmin, wxDialog)
EVT_BUTTON(ID_ADD_ACCOUNT_BUTTON, CAdmin::OnCreateNewAccount)
EVT_BUTTON(ID_SUPPR_ACCOUNT_BUTTON, CAdmin::OnEraseAccount)
EVT_BUTTON(BUTTON_SAVE, CAdmin::save_privileges)
EVT_LIST_ITEM_SELECTED(ID_LIST_ACCOUNT, CAdmin::OnSelectedAccount) 
EVT_CHECKBOX(ID_DWNL_FILE, CAdmin::WasModified) 
END_EVENT_TABLE()
