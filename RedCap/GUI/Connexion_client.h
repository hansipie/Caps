class Connexion_Client: public wxDialog
{
public:
		Connexion_Client(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos);
		wxTextCtrl *host;
		wxTextCtrl *port;
		MyFrame *MainFrame;

enum id_client_connexion {
	  TEXT,
	  TEXT_HOST,
	  TEXT_PORT,
	  BUTTON_CONNECT_CLIENT,
	  BUTTON_CANCEL,
  };

protected :
	void OnConnectClientButton(wxCommandEvent &event);
	void OnConnectCancelButton(wxCommandEvent &event);

private:
  DECLARE_EVENT_TABLE();
};
