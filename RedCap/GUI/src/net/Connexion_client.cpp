#include "general.h"

#pragma comment(lib, "ws2_32.lib")

Connexion_Client::Connexion_Client(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos)
:wxDialog(parent, id, title, pos, wxDefaultSize)
{
  MainFrame = (MyFrame *)parent;
  parent = this;
  
  wxBoxSizer *VMainBox = new wxBoxSizer( wxVERTICAL );
   wxStaticBox *Contour = new wxStaticBox( parent, -1, wxT("Connexion RedCap Console Client") );
    wxStaticBoxSizer *ContourSizer = new wxStaticBoxSizer( Contour, wxVERTICAL );
    ContourSizer->Add( 10, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
    wxStaticText *Client_Host_Label = new wxStaticText( parent, TEXT, wxT("Host :"), wxDefaultPosition, wxDefaultSize, 0 );
    ContourSizer->Add( Client_Host_Label, 0, wxALIGN_LEFT|wxALL, 5 );
    host = new wxTextCtrl( parent, TEXT_HOST, MainFrame->Client_Host, wxDefaultPosition, wxSize(150,-1), 0 );
    ContourSizer->Add( host, 0, wxALIGN_LEFT|wxALL, 5 );
    wxStaticText *Client_Port_Label = new wxStaticText( parent, TEXT, wxT("Port :"), wxDefaultPosition, wxDefaultSize, 0 );
    ContourSizer->Add( Client_Port_Label, 0, wxALIGN_LEFT|wxALL, 5 );
    port = new wxTextCtrl( parent, TEXT_PORT, MainFrame->Client_Port, wxDefaultPosition, wxSize(80,-1), 0 );
    ContourSizer->Add( port, 0, wxALIGN_LEFT|wxALL, 5 );
    ContourSizer->Add( 20, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
    wxBoxSizer *HBox = new wxBoxSizer( wxHORIZONTAL );
    wxButton *Client_Connect_Button = new wxButton( parent, BUTTON_CONNECT_CLIENT, wxT("Connect"), wxDefaultPosition, wxDefaultSize, 0 );
    Client_Connect_Button->SetDefault();
    HBox->Add( Client_Connect_Button, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    wxButton *Client_Cancel_Button = new wxButton( parent, BUTTON_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
    HBox->Add( Client_Cancel_Button, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    ContourSizer->Add( HBox, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
    VMainBox->Add( ContourSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 10 );
	SetSizer(VMainBox);
    VMainBox->Fit(this);
    VMainBox->SetSizeHints(this);
	SetAutoLayout(TRUE);
	Layout();
}

void Connexion_Client::OnConnectClientButton(wxCommandEvent& command)
{	
	MainFrame->Client_Host = host->GetValue();
	MainFrame->Client_Port = port->GetValue();
	EndModal(BUTTON_CONNECT_CLIENT);
}

void Connexion_Client::OnConnectCancelButton(wxCommandEvent& command)
{	
	EndModal(BUTTON_CANCEL);
	Close(TRUE);
}

BEGIN_EVENT_TABLE(Connexion_Client, wxDialog)
	EVT_BUTTON(BUTTON_CONNECT_CLIENT, Connexion_Client::OnConnectClientButton)
	EVT_BUTTON(BUTTON_CANCEL, Connexion_Client::OnConnectCancelButton)
END_EVENT_TABLE()
