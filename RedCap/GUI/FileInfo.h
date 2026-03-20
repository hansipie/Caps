
class FileInfo: public wxDialog
{
public:
	FileInfo(wxWindow* parent, wxWindowID id, wxString file_name, wxString other_info, const wxString& title, const wxPoint& pos);
		wxTextCtrl	*other_text;
		wxTextCtrl	*FileNameText;
		MyFrame		*MainFrame;
		wxString	file_save;
protected:
  void OnSaveInfo(wxCommandEvent &event);
  void OnCloseInfo(wxCommandEvent &event);
private:
  DECLARE_EVENT_TABLE();
public:
  enum id_file_info {
	  SAVE_INFO,
	  CLOSE_INFO,
	  ID_FILE_NAME,
  };
};
