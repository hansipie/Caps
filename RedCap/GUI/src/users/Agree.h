#define ID_AGREEMENT 10000
#define ID_AGREE 10001
#define ID_DISAGREE 10002

class CAgree: public wxDialog
{
public:
	CAgree(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, wxString agreement_txt);
	~CAgree(void);
	void OnCancel(wxCommandEvent& event);
	void OnOk(wxCommandEvent& event);

    MyFrame					*MainFrame;

private:
  DECLARE_EVENT_TABLE();

public:
  enum id_dialog_agree {
	BUTTON_CANCEL,
	BUTTON_OK
  };

};
