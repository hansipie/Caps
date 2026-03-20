#define LG_MAX_REQUETE 1024

class Connexion: public wxDialog
{
public:
	Connexion(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos);
		wxTextCtrl	*host;
		wxTextCtrl	*port;
		wxTextCtrl	*user;
		wxTextCtrl	*password;
		wxCheckBox	*connect_opt_ag;
		MyFrame		*MainFrame;

protected:
  void OnConnectServeurButton(wxCommandEvent &event);
  void OnConnectCancelButton(wxCommandEvent &event);
private:
  DECLARE_EVENT_TABLE();
public:
  // ID's
  enum id_dialog_connexion {
	  BUTTON_CONNECT,
	  BUTTON_SAVE,
	  BUTTON_CANCEL,
	  TEXT_USER,
	  TEXT_PASSWORD,
	  TEXT_HOST,
	  TEXT_PORT,
	  TEXT,
	  LISTBOX,
	  ID_AUTO_AGREE,
  };
};

