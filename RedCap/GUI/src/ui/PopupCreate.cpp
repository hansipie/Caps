#include "general.h"

PopupCreate::PopupCreate(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos)
:wxDialog(parent, id, title, pos, wxDefaultSize)
{
	
	MainFrame = (MyFrame *)parent;
	parent = this;

	wxBoxSizer *VMainBox = new wxBoxSizer( wxVERTICAL );
	wxStaticBox *Contour = new wxStaticBox( parent, -1, wxT("- Create Folder -") );
	wxStaticBoxSizer *ContourSizer = new wxStaticBoxSizer( Contour, wxVERTICAL );
	ContourSizer->Add( 10, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	wxStaticText *Client_Host_Label = new wxStaticText( parent, -1, wxT("New Folder Name :"), wxDefaultPosition, wxDefaultSize, 0 );
	ContourSizer->Add( Client_Host_Label, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	FolderNameText = new wxTextCtrl( parent, -1, wxT(""), wxDefaultPosition, wxSize(150,-1), 0 );
	ContourSizer->Add( FolderNameText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	ContourSizer->Add( 20, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	wxBoxSizer *HBox = new wxBoxSizer( wxHORIZONTAL );
	wxButton *Client_Connect_Button = new wxButton( parent, CREATE_FOLDER, wxT("Create"), wxDefaultPosition, wxDefaultSize, 0 );
	Client_Connect_Button->SetDefault();
	HBox->Add( Client_Connect_Button, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	wxButton *Client_Cancel_Button = new wxButton( parent, CANCEL_FOLDER, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	HBox->Add( Client_Cancel_Button, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	ContourSizer->Add( HBox, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	VMainBox->Add( ContourSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 10 );
	SetSizerAndFit(VMainBox);
}

void PopupCreate::OnCreateFolder(wxCommandEvent& command)
{	
	
	wxMessageBox("mkdir " + FolderNameText->GetValue());
	wxCommandEvent cmd;
	MainFrame->m_panel->ReloadExploServer(cmd);
	//output_message("mkdir " + FolderNameText->GetValue());
	EndModal(CREATE_FOLDER);
}

void PopupCreate::OnCancel(wxCommandEvent& command)
{	
	EndModal(CANCEL_FOLDER);
	Close(TRUE);
}

BEGIN_EVENT_TABLE(PopupCreate, wxDialog)
	EVT_BUTTON(CREATE_FOLDER, PopupCreate::OnCreateFolder)
	EVT_BUTTON(CANCEL_FOLDER, PopupCreate::OnCancel)
END_EVENT_TABLE()
