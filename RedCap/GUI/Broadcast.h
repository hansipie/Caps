class BroadcastDialog: public wxDialog
{
public:
		BroadcastDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos);
		void OnSendBroadcast(wxCommandEvent& command);
		
		wxTextCtrl		*message;
		wxString		msg_box;
		wxStaticBitmap	*warning_bmp;
		MyFrame			*MainFrame;

	
enum id_broadcast_dialog {	  
		ID_BROAD_TEXT,
		ID_SEND_BROADCAST
  };

protected :
	
private:
  DECLARE_EVENT_TABLE();

};
