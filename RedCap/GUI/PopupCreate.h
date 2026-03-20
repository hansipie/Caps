class PopupCreate: public wxDialog
{
public:
		PopupCreate(wxWindow* parent,  wxWindowID id, const wxString& title, const wxPoint& pos);
		wxTextCtrl *FolderNameText;
		MyFrame *MainFrame;

enum id_popup_create {
	  CREATE_FOLDER,
	  CANCEL_FOLDER,
  };

protected :
	void OnCreateFolder(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);

private:
  DECLARE_EVENT_TABLE();
};
