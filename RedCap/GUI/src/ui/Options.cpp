#include "general.h"


COptions::COptions(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos)
:wxDialog(parent, id, title, pos, wxDefaultSize)
{
 
    MainFrame = (MyFrame *)parent;
    parent = this;

	wxBoxSizer *SizerGlobal = new wxBoxSizer( wxVERTICAL );	
	opt_notebook = new wxNotebook(parent, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, 0);
	wxSizer *NotebookSizer = new wxBoxSizer(wxVERTICAL);
	NotebookSizer->Add(opt_notebook, 1, wxEXPAND);
	
	panel_option = new wxPanel(opt_notebook, -1);
	
	wxBoxSizer *SizerV = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *SizerH = new wxBoxSizer( wxHORIZONTAL );

	wxStaticText *user_label = new wxStaticText( panel_option, ID_USER_LABEL, wxT("User Name : "), 
						     wxDefaultPosition, wxDefaultSize, 0);
	SizerH->Add( user_label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	user_field = new wxTextCtrl( panel_option, ID_USER_FIELD, MainFrame->Name, wxDefaultPosition, wxSize(100, -1), 0);
	SizerH->Add( user_field, 0, wxALIGN_CENTER_VERTICAL |  wxALL, 5);
	
	SizerV->Add( SizerH, 0, wxALIGN_LEFT|wxALL, 5 );
	
	opt_autoagree = new wxCheckBox( panel_option, ID_OPT_AUTOAGREE, wxT("Auto Agreement"), 
					wxDefaultPosition, wxDefaultSize, 0 );
	opt_autoagree->SetValue(MainFrame->auto_agree);
	SizerV->Add( opt_autoagree, 0, 0|wxALL, 5 );
	
	opt_sjlc = new wxCheckBox( panel_option, ID_OPT_SJLC, wxT("Show Join/Leave chat"), 
				   wxDefaultPosition, wxDefaultSize, 0 );
	opt_sjlc->SetValue(MainFrame->mess_join_leave_chat);
	SizerV->Add( opt_sjlc, 0, 0|wxALL, 5 );
	
	opt_refus_privchat = new wxCheckBox( panel_option, ID_OPT_REFUS_PRIVCHAT, wxT("Refuse Private Message"), 
					     wxDefaultPosition, wxDefaultSize, 0 );
	opt_refus_privchat->SetValue(!MainFrame->accept_priv_chat);
	SizerV->Add( opt_refus_privchat, 0, 0|wxALL, 5 );
	
	opt_autoresponse = new wxCheckBox( panel_option, ID_OPT_AUTORESPONSE, wxT("Automatic Response"), 
					   wxDefaultPosition, wxDefaultSize, 0 );
	opt_autoresponse->SetValue(MainFrame->automatic_response);
	SizerV->Add( opt_autoresponse, 0, 0|wxALL, 5 );
	
	panel_option->SetSizer(SizerV);
	opt_notebook->AddPage(panel_option, wxT("Options"));

	wxPanel *panel_crypto = new wxPanel(opt_notebook, -1);

	wxBoxSizer *SizerCryptoV = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *SizerCryptoH = new wxBoxSizer( wxHORIZONTAL );

	opt_crypto = new wxCheckBox( panel_crypto, ID_OPT_CRYPTO, wxT("Activate Security Data"), 
					wxDefaultPosition, wxDefaultSize, 0 );
	opt_crypto->SetValue(MainFrame->opt_crypto);
	SizerCryptoV->Add( opt_crypto, 0, wxALIGN_LEFT|wxALL, 5 );

	wxStaticText *crypto_label = new wxStaticText( panel_crypto, ID_CRYPTO_LABEL, wxT("Password Key : "), 
						     wxDefaultPosition, wxDefaultSize, 0);
	SizerCryptoH->Add( crypto_label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	PassCrypto_field = new wxTextCtrl( panel_crypto, ID_PASSCRYPTO_FIELD, MainFrame->Pass_crypto, wxDefaultPosition, wxSize(100, -1), wxTE_PASSWORD);
	SizerCryptoH->Add( PassCrypto_field, 0, wxALIGN_CENTER_VERTICAL |  wxALL , 5);
	
	SizerCryptoV->Add( SizerCryptoH, 0, wxALIGN_LEFT|wxALL, 5 );
	
	panel_crypto->SetSizer(SizerCryptoV);
	opt_notebook->AddPage(panel_crypto, wxT("Data Secutity"));
	
	SizerGlobal->Add(NotebookSizer, 1, wxEXPAND|wxALL, 5);

	wxBoxSizer *SizerButton = new wxBoxSizer(wxHORIZONTAL);
	wxButton *ok_opt_button = new wxButton(this, BUTTON_OK, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0);
	SizerButton->Add(ok_opt_button, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxButton *cancel_opt_button = new wxButton(this, BUTTON_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	SizerButton->Add(cancel_opt_button, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	SizerGlobal->Add(SizerButton, 0, wxALIGN_CENTER|wxALL, 5);

	SetSizerAndFit(SizerGlobal);
}

void COptions::OnCancel(wxCommandEvent &event)
{
	EndModal(BUTTON_CANCEL);
	Close(TRUE);
}
void COptions::OnOk(wxCommandEvent &event)
{
	
	MainFrame->Name = user_field->GetValue();
	MainFrame->Pass_crypto = PassCrypto_field->GetValue();

	if (opt_sjlc->GetValue())
		MainFrame->mess_join_leave_chat = TRUE;
	else
		MainFrame->mess_join_leave_chat = FALSE;

	
	if (opt_refus_privchat->GetValue())
		MainFrame->accept_priv_chat = FALSE;
	else
		MainFrame->accept_priv_chat = TRUE;

	if (opt_autoresponse->GetValue())
		MainFrame->automatic_response = TRUE;
	else
		MainFrame->automatic_response = FALSE;

	if (opt_autoagree->GetValue())
		MainFrame->auto_agree = TRUE;
	else
		MainFrame->auto_agree = FALSE;
	
	if (opt_crypto->GetValue())
		MainFrame->opt_crypto = TRUE;
	else
		MainFrame->opt_crypto = FALSE;

	MainFrame->output_message("set_user " + MainFrame->Name + " " + MainFrame->Icone);
	
	EndModal(BUTTON_OK);
	Close(TRUE);
}

COptions::~COptions(void)
{
}


BEGIN_EVENT_TABLE(COptions, wxDialog)
EVT_BUTTON(BUTTON_CANCEL, COptions::OnCancel)
EVT_BUTTON(BUTTON_OK, COptions::OnOk)
END_EVENT_TABLE()
