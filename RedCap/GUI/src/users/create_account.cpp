#include "general.h"

CCreateAccount::CCreateAccount(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos)
:wxDialog(parent, id, title, pos, wxDefaultSize)
{
	MainFrame = (MyFrame *)parent;
    parent = this;

	wxBoxSizer *VMainBox = new wxBoxSizer(wxVERTICAL);
	wxStaticBox *Contour = new wxStaticBox(parent, -1, wxT("New User Account Information"));
	wxStaticBoxSizer *ContourSizer = new wxStaticBoxSizer(Contour, wxVERTICAL);

	wxFlexGridSizer *FormGrid = new wxFlexGridSizer(3, 2, 5, 10);
	FormGrid->AddGrowableCol(1, 1);

	wxStaticText *name_label = new wxStaticText(parent, ID_TEXT, wxT("Name :"), wxDefaultPosition, wxDefaultSize, 0);
	name_field = new wxTextCtrl(parent, ID_TEXTCTRL, wxT(""), wxDefaultPosition, wxSize(130,-1), 0);
	FormGrid->Add(name_label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
	FormGrid->Add(name_field, 0, wxEXPAND|wxALL, 3);

	wxStaticText *login_label = new wxStaticText(parent, ID_TEXT, wxT("Login :"), wxDefaultPosition, wxDefaultSize, 0);
	login_field = new wxTextCtrl(parent, ID_TEXTCTRL, wxT(""), wxDefaultPosition, wxSize(130,-1), 0);
	FormGrid->Add(login_label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
	FormGrid->Add(login_field, 0, wxEXPAND|wxALL, 3);

	wxStaticText *password_label = new wxStaticText(parent, ID_TEXT, wxT("Password :"), wxDefaultPosition, wxDefaultSize, 0);
	password_field = new wxTextCtrl(parent, ID_TEXTCTRL, wxT(""), wxDefaultPosition, wxSize(130,-1), wxTE_PASSWORD);
	FormGrid->Add(password_label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
	FormGrid->Add(password_field, 0, wxEXPAND|wxALL, 3);

	ContourSizer->Add(FormGrid, 0, wxEXPAND|wxALL, 5);

	wxBoxSizer *ButtonBox = new wxBoxSizer(wxHORIZONTAL);
	wxButton *create_btn = new wxButton(parent, ID_CREATED_ACCOUNT, wxT("Create"), wxDefaultPosition, wxDefaultSize, 0);
	create_btn->SetDefault();
	ButtonBox->Add(create_btn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxButton *cancel_btn = new wxButton(parent, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	ButtonBox->Add(cancel_btn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	ContourSizer->Add(ButtonBox, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	VMainBox->Add(ContourSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 10);
	SetSizerAndFit(VMainBox);
}

void CCreateAccount::OnConfirmNewAccount(wxCommandEvent& command)
{	
	if (name_field->GetValue().IsEmpty() || login_field->GetValue().IsEmpty())
	{
		wxMessageDialog MsgDlg(this, "You must fill in at least the login name and the user name to create an Account !", "Save Adminstration Changes", wxYES_DEFAULT | wxICON_EXCLAMATION);
		MsgDlg.ShowModal();
	}
	else
	{
		wxString new_pass = password_field->GetValue();
		if (new_pass.IsEmpty())
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
