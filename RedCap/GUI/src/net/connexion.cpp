#include "general.h"

//#pragma comment(lib, "ws2_32.lib")

Connexion::Connexion(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos)
:wxDialog(parent, id, title, pos, wxSize(250, 430))
{
	MainFrame = (MyFrame *)parent;
	parent = this;

	wxFlexGridSizer *sizer0 = new wxFlexGridSizer( 1, 0, 0 );
    wxFlexGridSizer *sizer1 = new wxFlexGridSizer( 2, 0, 0 );

    wxStaticText *host_label = new wxStaticText( parent, TEXT, wxT("Host or URL :"), wxDefaultPosition, wxDefaultSize, 0 );
    sizer1->Add( host_label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *port_label = new wxStaticText( parent, TEXT, wxT("Port :"), wxDefaultPosition, wxDefaultSize, 0 );
    sizer1->Add( port_label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	host = new wxTextCtrl( parent, TEXT_HOST, MainFrame->Serv_Host, wxDefaultPosition, wxSize(175,-1), 0 );
    sizer1->Add( host, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    port = new wxTextCtrl( parent, TEXT_PORT, MainFrame->Serv_Port, wxDefaultPosition, wxSize(50,-1), 0 );
    sizer1->Add( port, 0, wxALIGN_CENTER|wxALL, 5 );

    sizer0->Add( sizer1, 0, wxALIGN_CENTER|wxALL, 0 );

    wxFlexGridSizer *sizer2 = new wxFlexGridSizer( 2, 0, 0 );

    wxStaticText *user_label = new wxStaticText( parent, TEXT, wxT("User :"), wxDefaultPosition, wxDefaultSize, 0 );
    sizer2->Add( user_label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *password_label = new wxStaticText( parent, TEXT, wxT("Password :"), wxDefaultPosition, wxDefaultSize, 0 );
    sizer2->Add( password_label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    user = new wxTextCtrl( parent, TEXT, MainFrame->Serv_User, wxDefaultPosition, wxSize(112,-1), 0 );
    sizer2->Add( user, 0, wxGROW|wxALL, 5 );

    password = new wxTextCtrl( parent, TEXT_PASSWORD, MainFrame->Serv_Passwd, wxDefaultPosition, wxSize(112,-1), wxTE_PASSWORD );
    sizer2->Add( password, 0, wxALIGN_CENTER|wxALL, 5 );

    sizer0->Add( sizer2, 0, wxALIGN_CENTER|wxALL, 0 );

	wxBoxSizer *sizer22 = new wxBoxSizer( wxVERTICAL );
	connect_opt_ag = new wxCheckBox( parent, ID_AUTO_AGREE, wxT("Auto Agreement"), wxDefaultPosition, wxDefaultSize, 0 );
    sizer22->Add( connect_opt_ag, 0, wxALIGN_LEFT|wxALL, 5 );
	sizer0->Add( sizer22, 0, wxALIGN_LEFT|wxALL, 0 );

    wxBoxSizer *sizer3 = new wxBoxSizer( wxHORIZONTAL );

    wxButton *save_button = new wxButton( parent, BUTTON_SAVE, wxT("Save..."), wxDefaultPosition, wxDefaultSize, 0 );
    sizer3->Add( save_button, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxButton *connect_button = new wxButton( parent, BUTTON_CONNECT, wxT("Connect.."), wxDefaultPosition, wxDefaultSize, 0 );
    sizer3->Add( connect_button, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    sizer0->Add( sizer3, 0, wxALIGN_CENTER|wxALL, 5 );

    wxString strs14[] = 
    {
        wxT("ListItem")
    };
    MainFrame->list_bookmark = new wxListBox( parent, ID_LISTBOX, wxDefaultPosition, wxSize(230,180), 1, strs14, wxLB_SINGLE );
    sizer0->Add( MainFrame->list_bookmark, 0, wxALIGN_CENTER|wxALL, 5 );
    wxButton *cancel_connect_button = new wxButton( parent, BUTTON_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    sizer0->Add( cancel_connect_button, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	SetSizer(sizer0);
	SetAutoLayout(TRUE);
	Layout();
}

void Connexion::OnConnectServeurButton(wxCommandEvent& command)
{	
	MainFrame->Serv_User = user->GetValue();
	MainFrame->Serv_Passwd = password->GetValue();
	MainFrame->Serv_Host = host->GetValue();
	MainFrame->Serv_Port = port->GetValue();
	MainFrame->connect_opt_ag = connect_opt_ag->GetValue();

	EndModal(BUTTON_CONNECT);
}

void Connexion::OnConnectCancelButton(wxCommandEvent& command)
{
	EndModal(BUTTON_CANCEL);
	Close(TRUE);
}


BEGIN_EVENT_TABLE(Connexion, wxDialog)
	EVT_BUTTON(BUTTON_CONNECT, Connexion::OnConnectServeurButton)
	EVT_BUTTON(BUTTON_CANCEL, Connexion::OnConnectCancelButton)
END_EVENT_TABLE()
