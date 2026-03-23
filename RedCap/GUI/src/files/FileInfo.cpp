#include "general.h"

FileInfo::FileInfo(wxWindow* parent, wxWindowID id, wxString file_name, wxString other_info, const wxString& title, const wxPoint& pos)
:wxDialog(parent, id, title, pos, wxDefaultSize)
{
	MainFrame = (MyFrame *)parent;
	parent = this;
  
	file_save = file_name;
    wxBoxSizer *VMainBox = new wxBoxSizer( wxVERTICAL );
    wxStaticBox *Contour = new wxStaticBox( parent, -1, wxT("File Informations") );
    wxStaticBoxSizer *ContourSizer = new wxStaticBoxSizer( Contour, wxVERTICAL );
    ContourSizer->Add( 10, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
    FileNameText = new wxTextCtrl( parent, ID_FILE_NAME, file_name, wxDefaultPosition, wxSize(150,-1), 0 );
    ContourSizer->Add( FileNameText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	wxStaticText *OtherInfo = new wxStaticText( parent, -1, other_info, wxDefaultPosition, wxDefaultSize, 0 );
    ContourSizer->Add( OtherInfo, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	ContourSizer->Add( 20, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxBoxSizer *HBox = new wxBoxSizer( wxHORIZONTAL);
	wxButton *SaveButton = new wxButton( parent, SAVE_INFO, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
    SaveButton->SetDefault();
    HBox->Add( SaveButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxButton *CloseButton = new wxButton( parent, CLOSE_INFO, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
    HBox->Add( CloseButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    ContourSizer->Add( HBox, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
    VMainBox->Add( ContourSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 10 );
	SetSizerAndFit(VMainBox);
}

void FileInfo::OnSaveInfo(wxCommandEvent& command)
{	
	wxString new_name;
	new_name = FileNameText->GetValue();
	//	wxMessageBox("rename \"" + file_save + "\" \"" + new_name + "\"");
	MainFrame->output_message("rename \"" + file_save + "\" \"" + new_name + "\"");
	wxCommandEvent cmd;
	MainFrame->m_panel->ReloadExploServer(cmd);
	EndModal(SAVE_INFO);
	Close(TRUE);
}

void FileInfo::OnCloseInfo(wxCommandEvent& command)
{	
	EndModal(CLOSE_INFO);
	Close(TRUE);
}

BEGIN_EVENT_TABLE(FileInfo, wxDialog)
	EVT_BUTTON(SAVE_INFO, FileInfo::OnSaveInfo)
	EVT_BUTTON(CLOSE_INFO, FileInfo::OnCloseInfo)
END_EVENT_TABLE()
