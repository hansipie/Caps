enum
{
  ID_USER_LABEL,
  ID_USER_FIELD,
  ID_OPT_AUTOAGREE,
  ID_OPT_SJLC,
  ID_OPT_REFUS_PRIVCHAT,
  ID_OPT_AUTORESPONSE,
  ID_OPT_CRYPTO,
  ID_PASSCRYPTO_FIELD,
  ID_CRYPTO_LABEL
  
};

class COptions: public wxDialog
{
public:
	COptions(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos);
	~COptions(void);

	void create_general();

    MyFrame					*MainFrame;
	wxNotebook				*opt_notebook;
	wxPanel					*panel_option;


	wxTextCtrl *user_field;
	wxTextCtrl *PassCrypto_field;
	wxCheckBox *opt_crypto;
	wxCheckBox *opt_sjlc;
	wxCheckBox *opt_refus_privchat;
	wxCheckBox *opt_autoresponse;
	wxCheckBox *opt_autoagree;

protected:
	void OnCancel(wxCommandEvent &event);
	void OnOk(wxCommandEvent &event);

private:
  DECLARE_EVENT_TABLE();

public:
  enum id_dialog_options {
	ID_OPTNOTEBOOK=   wxID_HIGHEST + 1,
	BUTTON_CANCEL,
	BUTTON_OK
};

};
