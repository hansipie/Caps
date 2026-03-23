#include "general.h"

CAdmin::CAdmin(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos)
:wxDialog(parent, id, title, pos, wxDefaultSize)
{
	MainFrame = (MyFrame *)parent;
    parent = this;
	save_flag = 0;
	wxBitmap AdminBitmaps[2];
	AdminBitmaps[0] = wxBitmap("./icons/add_user_account.bmp",wxBITMAP_TYPE_BMP);
	AdminBitmaps[1] = wxBitmap("./icons/suppr_user_account.bmp",wxBITMAP_TYPE_BMP);

	wxMask *mask0 = new wxMask(AdminBitmaps[0], *wxWHITE);
    AdminBitmaps[0].SetMask(mask0);
	wxMask *mask1 = new wxMask(AdminBitmaps[1], *wxWHITE);
    AdminBitmaps[1].SetMask(mask1);

	add_account_button = new wxBitmapButton(parent, ID_ADD_ACCOUNT_BUTTON,
								AdminBitmaps[0],
								wxDefaultPosition,
								wxSize(30, 30));
	suppr_account_button = new wxBitmapButton(parent, ID_SUPPR_ACCOUNT_BUTTON,
								AdminBitmaps[1],
								wxDefaultPosition,
								wxSize(30, 30));

//------------------------------------------------------------------------------------------------------------

	BitmapUser = new wxStaticBitmap(
									parent, -1,
									wxBitmap("./icons/Accounts.bmp", wxBITMAP_TYPE_BMP),
									wxDefaultPosition,
									wxDefaultSize
									);

	list_account = new wxListCtrl(parent,
										ID_LIST_ACCOUNT,
										wxDefaultPosition, wxSize(230, -1),
										wxSUNKEN_BORDER | wxLC_REPORT);

	list_account->InsertColumn( 0, wxT("Login"), wxLIST_FORMAT_LEFT, 110 );

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

	wxStaticBox *InfoContour = new wxStaticBox(parent, -1, wxT("User Account Information"));
	wxStaticBoxSizer *InfoBox = new wxStaticBoxSizer(InfoContour, wxVERTICAL);

	wxBoxSizer *NameRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *name_label = new wxStaticText(parent, ID_TEXT, wxT("Name :"), wxDefaultPosition, wxDefaultSize, 0);
	UserName = new wxTextCtrl(parent, ID_TEXTCTRL, wxT(""), wxDefaultPosition, wxSize(130,-1), 0);
	NameRow->Add(name_label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	NameRow->Add(UserName, 1, wxEXPAND|wxALL, 5);
	InfoBox->Add(NameRow, 0, wxEXPAND|wxALL, 2);

	wxBoxSizer *LoginRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *login_label = new wxStaticText(parent, ID_TEXT, wxT("Login :"), wxDefaultPosition, wxDefaultSize, 0);
	login = new wxTextCtrl(parent, ID_TEXTCTRL, wxT(""), wxDefaultPosition, wxSize(130,-1), 0);
	LoginRow->Add(login_label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	LoginRow->Add(login, 1, wxEXPAND|wxALL, 5);
	InfoBox->Add(LoginRow, 0, wxEXPAND|wxALL, 2);

	wxBoxSizer *PasswordRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *password_label = new wxStaticText(parent, ID_TEXT, wxT("Password :"), wxDefaultPosition, wxDefaultSize, 0);
	password = new wxTextCtrl(parent, ID_TEXTCTRL, wxT(""), wxDefaultPosition, wxSize(130,-1), wxTE_PASSWORD);
	PasswordRow->Add(password_label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	PasswordRow->Add(password, 1, wxEXPAND|wxALL, 5);
	InfoBox->Add(PasswordRow, 0, wxEXPAND|wxALL, 2);

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

	rights_notebook = new wxNotebook(parent, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, 0);
	rights_notebook->SetMinSize(wxSize(270, 200));

	panel_FilesRights = new wxPanel(rights_notebook, -1);
	wxFlexGridSizer *FilesSizer = new wxFlexGridSizer(0, 2, 2, 10);

	downloadfile = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can download Files"), wxDefaultPosition, wxDefaultSize, 0 );
	uploadfile = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can upload Files"), wxDefaultPosition, wxDefaultSize, 0 );
	uploadanywhere = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can upload Anywhere"), wxDefaultPosition, wxDefaultSize, 0 );
	deletefile = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can delete Files"), wxDefaultPosition, wxDefaultSize, 0 );
	renamefile = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can rename Files"), wxDefaultPosition, wxDefaultSize, 0 );
	movefile = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can move Files"), wxDefaultPosition, wxDefaultSize, 0 );
	getfilecomment = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can comment Files"), wxDefaultPosition, wxDefaultSize, 0 );
	createfolder = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can create Folders"), wxDefaultPosition, wxDefaultSize, 0 );
	renamefolder = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can rename Folders"), wxDefaultPosition, wxDefaultSize, 0 );
	deletefolder = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can delete Folders"), wxDefaultPosition, wxDefaultSize, 0 );
	movefolder = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can move Folders"), wxDefaultPosition, wxDefaultSize, 0 );
	setfoldercomment = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can comment Folders"), wxDefaultPosition, wxDefaultSize, 0 );
	makealias = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can make Aliases"), wxDefaultPosition, wxDefaultSize, 0 );
	viewdropboxes = new wxCheckBox( panel_FilesRights, ID_DWNL_FILE, wxT("Can view Drop Boxes"), wxDefaultPosition, wxDefaultSize, 0 );

	FilesSizer->Add(downloadfile, 0, wxALL, 3);
	FilesSizer->Add(createfolder, 0, wxALL, 3);
	FilesSizer->Add(uploadfile, 0, wxALL, 3);
	FilesSizer->Add(renamefolder, 0, wxALL, 3);
	FilesSizer->Add(uploadanywhere, 0, wxALL, 3);
	FilesSizer->Add(deletefolder, 0, wxALL, 3);
	FilesSizer->Add(deletefile, 0, wxALL, 3);
	FilesSizer->Add(movefolder, 0, wxALL, 3);
	FilesSizer->Add(renamefile, 0, wxALL, 3);
	FilesSizer->Add(setfoldercomment, 0, wxALL, 3);
	FilesSizer->Add(movefile, 0, wxALL, 3);
	FilesSizer->Add(makealias, 0, wxALL, 3);
	FilesSizer->Add(getfilecomment, 0, wxALL, 3);
	FilesSizer->Add(viewdropboxes, 0, wxALL, 3);

	panel_FilesRights->SetSizer(FilesSizer);
	rights_notebook->AddPage(panel_FilesRights, wxT("Files"));

	panel_UserRights = new wxPanel(rights_notebook, -1);
	wxBoxSizer *UserSizer = new wxBoxSizer(wxVERTICAL);

	createuser = new wxCheckBox( panel_UserRights, ID_DWNL_FILE, wxT("Can cresate Accounts"), wxDefaultPosition, wxDefaultSize, 0 );
	modifyuser = new wxCheckBox( panel_UserRights, ID_DWNL_FILE, wxT("Can modify Accounts"), wxDefaultPosition, wxDefaultSize, 0 );
	deleteuser = new wxCheckBox( panel_UserRights, ID_DWNL_FILE, wxT("Can delete Accounts"), wxDefaultPosition, wxDefaultSize, 0 );
	showinlist = new wxCheckBox( panel_UserRights, ID_DWNL_FILE, wxT("Can read Accounts"), wxDefaultPosition, wxDefaultSize, 0 );
	openuser = new wxCheckBox( panel_UserRights, ID_DWNL_FILE, wxT("Can get User Info"), wxDefaultPosition, wxDefaultSize, 0 );
	changeownpassword = new wxCheckBox( panel_UserRights, ID_DWNL_FILE, wxT("Can change own password"), wxDefaultPosition, wxDefaultSize, 0 );

	UserSizer->Add(createuser, 0, wxALL, 3);
	UserSizer->Add(modifyuser, 0, wxALL, 3);
	UserSizer->Add(deleteuser, 0, wxALL, 3);
	UserSizer->Add(showinlist, 0, wxALL, 3);
	UserSizer->Add(openuser, 0, wxALL, 3);
	UserSizer->Add(changeownpassword, 0, wxALL, 3);

	panel_UserRights->SetSizer(UserSizer);
	rights_notebook->AddPage(panel_UserRights, wxT("User"));

	panel_ChatRights = new wxPanel(rights_notebook, -1);
	wxBoxSizer *ChatSizer = new wxBoxSizer(wxVERTICAL);

	readchat = new wxCheckBox( panel_ChatRights, ID_DWNL_FILE, wxT("Can read Chat"), wxDefaultPosition, wxDefaultSize, 0 );
	sendchat = new wxCheckBox( panel_ChatRights, ID_DWNL_FILE, wxT("Can send Accounts"), wxDefaultPosition, wxDefaultSize, 0 );
	openchat = new wxCheckBox( panel_ChatRights, ID_DWNL_FILE, wxT("Can initiate Private Chat"), wxDefaultPosition, wxDefaultSize, 0 );
	closechat = new wxCheckBox( panel_ChatRights, ID_DWNL_FILE, wxT("Can close Chat"), wxDefaultPosition, wxDefaultSize, 0 );
	sendprivatemessage = new wxCheckBox( panel_ChatRights, ID_DWNL_FILE, wxT("Can send Messages"), wxDefaultPosition, wxDefaultSize, 0 );
	broadcast = new wxCheckBox( panel_ChatRights, ID_DWNL_FILE, wxT("Can Broadcast"), wxDefaultPosition, wxDefaultSize, 0 );

	ChatSizer->Add(readchat, 0, wxALL, 3);
	ChatSizer->Add(sendchat, 0, wxALL, 3);
	ChatSizer->Add(openchat, 0, wxALL, 3);
	ChatSizer->Add(closechat, 0, wxALL, 3);
	ChatSizer->Add(sendprivatemessage, 0, wxALL, 3);
	ChatSizer->Add(broadcast, 0, wxALL, 3);

	panel_ChatRights->SetSizer(ChatSizer);
	rights_notebook->AddPage(panel_ChatRights, wxT("Chat"));

	panel_MiscellaneousRights = new wxPanel(rights_notebook, -1);
	wxBoxSizer *MiscSizer = new wxBoxSizer(wxVERTICAL);

	anyname = new wxCheckBox( panel_MiscellaneousRights, ID_DWNL_FILE, wxT("Can use any name"), wxDefaultPosition, wxDefaultSize, 0 );
	noagreement = new wxCheckBox( panel_MiscellaneousRights, ID_DWNL_FILE, wxT("Don't show Agreement"), wxDefaultPosition, wxDefaultSize, 0 );
	DisconnectUser = new wxCheckBox( panel_MiscellaneousRights, ID_DWNL_FILE, wxT("Can Disconnect User"), wxDefaultPosition, wxDefaultSize, 0 );
	GetClientInfo = new wxCheckBox( panel_MiscellaneousRights, ID_DWNL_FILE, wxT("Get Client Info"), wxDefaultPosition, wxDefaultSize, 0 );

	MiscSizer->Add(anyname, 0, wxALL, 3);
	MiscSizer->Add(noagreement, 0, wxALL, 3);
	MiscSizer->Add(DisconnectUser, 0, wxALL, 3);
	MiscSizer->Add(GetClientInfo, 0, wxALL, 3);

	panel_MiscellaneousRights->SetSizer(MiscSizer);
	rights_notebook->AddPage(panel_MiscellaneousRights, wxT("Miscellaneous"));

//-------------------------------------------------------------------------------------------------------------
// Buttons
//-------------------------------------------------------------------------------------------------------------

	wxButton *save_btn = new wxButton(parent, BUTTON_SAVE, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0);
	wxButton *cancel_btn = new wxButton(parent, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);

//-------------------------------------------------------------------------------------------------------------
// Sizer assembly
//-------------------------------------------------------------------------------------------------------------

	wxBoxSizer *VMainBox = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *TopToolbar = new wxBoxSizer(wxHORIZONTAL);
	TopToolbar->Add(add_account_button, 0, wxALL, 3);
	TopToolbar->Add(suppr_account_button, 0, wxALL, 3);
	VMainBox->Add(TopToolbar, 0, wxLEFT|wxTOP, 5);

	wxBoxSizer *ContentBox = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer *LeftPanel = new wxBoxSizer(wxVERTICAL);
	LeftPanel->Add(BitmapUser, 0, wxALL, 3);
	LeftPanel->Add(list_account, 1, wxEXPAND|wxALL, 3);
	ContentBox->Add(LeftPanel, 0, wxEXPAND|wxALL, 5);

	wxBoxSizer *RightPanel = new wxBoxSizer(wxVERTICAL);
	RightPanel->Add(InfoBox, 0, wxEXPAND|wxALL, 5);
	RightPanel->Add(rights_notebook, 1, wxEXPAND|wxALL, 5);
	ContentBox->Add(RightPanel, 1, wxEXPAND|wxALL, 5);

	VMainBox->Add(ContentBox, 1, wxEXPAND|wxALL, 5);

	wxBoxSizer *ButtonRow = new wxBoxSizer(wxHORIZONTAL);
	ButtonRow->Add(save_btn, 0, wxALL, 5);
	ButtonRow->Add(cancel_btn, 0, wxALL, 5);
	VMainBox->Add(ButtonRow, 0, wxALIGN_RIGHT|wxALL, 5);

	SetSizerAndFit(VMainBox);
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

	long item = list_account->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (item != -1)
		account = list_account->GetItemText(item);
	suppr_message = "Are you sure you want to delete the user \"" + account + "\" ?";

#ifdef __WIN32__
	int suppr;
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

void CAdmin::OnSelectedAccount(wxListEvent& command)
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
			if (pass.IsEmpty())
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
			if (pass.IsEmpty())
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
