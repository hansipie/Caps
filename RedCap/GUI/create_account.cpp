#include "general.h"

CCreateAccount::CCreateAccount(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos)
:wxDialog(parent, id, title, pos, wxSize(290, 210))
{
	MainFrame = (MyFrame *)parent;
    parent = this;
	
	wxStaticBox *info;
	info = new wxStaticBox( parent, -1, wxT("New User Account Information"), wxPoint(10, 10), wxSize(265, 130));
    
    wxStaticText *name_label;
	name_label = new wxStaticText( parent, ID_TEXT, wxT("Name :"), wxPoint(20, 40), wxDefaultSize, 0 );
    name_field = new wxTextCtrl( parent, ID_TEXTCTRL, wxT(""), wxPoint(130, 40), wxSize(130,-1), 0 );

    wxStaticText *login_label;
	login_label = new wxStaticText( parent, ID_TEXT, wxT("Login :"), wxPoint(20, 70), wxDefaultSize, 0 );
    login_field = new wxTextCtrl( parent, ID_TEXTCTRL, wxT(""), wxPoint(130, 70), wxSize(130,-1), 0 );

	wxStaticText *password_label;
	password_label = new wxStaticText( parent, ID_TEXT, wxT("Password :"), wxPoint(20, 100), wxDefaultSize, 0 );
	password_field = new wxTextCtrl( parent, ID_TEXTCTRL, wxT(""), wxPoint(130, 100), wxSize(130,-1), wxTE_PASSWORD );

	wxButton *create_account_button;
	create_account_button = new wxButton( parent, ID_CREATED_ACCOUNT, wxT("Create"), wxPoint(120, 150), wxDefaultSize,0);
	wxButton *cancel_account_button;
	cancel_account_button = new wxButton( parent, wxID_CANCEL, wxT("Cancel"), wxPoint(200, 150), wxDefaultSize,0);
}

void CCreateAccount::OnConfirmNewAccount(wxCommandEvent& command)
{	
	if (IsEmpty(name_field->GetValue()) || IsEmpty(login_field->GetValue()))
	{
		wxMessageDialog MsgDlg(this, "You must fill in at least the login name and the user name to create an Account !", "Save Adminstration Changes", wxYES_DEFAULT | wxICON_EXCLAMATION);
		MsgDlg.ShowModal();
	}
	else
	{
		wxString new_pass = password_field->GetValue();
		if (IsEmpty(new_pass))
			new_pass = " ";
		CreateAccountCmd = "new_user \"" + login_field->GetValue() +"\" \""+ new_pass + "\" \"" +name_field->GetValue() +"\" \"0000000000000000000000000000000000000000000000000000000000000000\"";
		//wxMessageBox(CreateAccountCmd);

		EndModal(ID_CREATED_ACCOUNT);
				//MainFrame->output_message(CreateAccountCmd);
	}
	//MainFrame->Client_Host = name_field->GetValue();
	//MainFrame->Client_Port = login_field->GetValue();
	//EndModal(ID_CREATED_ACCOUNT);
	//wxMessageBox(CreateAccountCmd);
}

CCreateAccount::~CCreateAccount(void)
{
}

BEGIN_EVENT_TABLE(CCreateAccount, wxDialog)
EVT_BUTTON(ID_CREATED_ACCOUNT, CCreateAccount::OnConfirmNewAccount)
END_EVENT_TABLE()
