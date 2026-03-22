#include "general.h"

BroadcastDialog::BroadcastDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos)
:wxDialog(parent, id, title, pos, wxSize(360, 150))
{
	MainFrame = (MyFrame *)parent;
	parent = this;
	
	msg_box = MainFrame->Broadcast_String;

	 wxBoxSizer *sizerV = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *sizerH = new wxBoxSizer( wxHORIZONTAL );

	wxBitmap pic = wxBitmap("./icons/warning.bmp",wxBITMAP_TYPE_BMP);
	wxMask *mask = new wxMask(pic, *wxWHITE);
    pic.SetMask(mask);
	warning_bmp = new wxStaticBitmap(parent, -1,
									pic,
									wxPoint(0,0),
									wxDefaultSize
									);
	
    sizerH->Add( warning_bmp, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
    message = new wxTextCtrl( parent, ID_BROAD_TEXT, msg_box, wxDefaultPosition, wxSize(240,60), wxTE_MULTILINE | wxTE_RICH2 );
	if (MainFrame->BroadcastFlag == 2)
		message->Clear();
	sizerH->Add( message, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    sizerV->Add( sizerH, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    wxButton *button = new wxButton( parent, ID_SEND_BROADCAST, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	button->SetDefault();  
	sizerV->Add( button, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	SetSizer(sizerV);
	SetAutoLayout(TRUE);
	Layout();
}

void BroadcastDialog::OnSendBroadcast(wxCommandEvent& command)
{
	//wxString msg;
	//msg = 
	if (MainFrame->BroadcastFlag == 2)
	{
		msg_box = message->GetValue();
		MainFrame->output_message("send_broadcast \"" + msg_box + "\"");
		message->Clear();
	}
	message->Clear();
	EndModal(ID_SEND_BROADCAST);
	
}
