#include "general.h"
#include "ConfirmBox.h"


CConfirmBox::CConfirmBox(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, wxString Message, wxWindowID id_ConfirmYes, int flag)
:wxDialog(parent, id, title, pos, wxDefaultSize)
{
	parent = NULL;
	wxBitmap pic = wxBitmap("./icons/important.bmp", wxBITMAP_TYPE_BMP);
	wxMask *mask = new wxMask(pic, *wxWHITE);
	pic.SetMask(mask);
	wxStaticBitmap *warning_bmp = new wxStaticBitmap(this, -1, pic, wxDefaultPosition, wxDefaultSize);

	wxBoxSizer *HMainSizer = new wxBoxSizer(wxHORIZONTAL);
	HMainSizer->Add(warning_bmp, 0, wxALIGN_CENTER_VERTICAL|wxALL, 10);

	wxBoxSizer *VRight = new wxBoxSizer(wxVERTICAL);
	wxStaticText *Message_label = new wxStaticText(this, ID_TEXT, Message, wxDefaultPosition, wxDefaultSize, 0);
	VRight->Add(Message_label, 0, wxALL, 5);

	wxBoxSizer *ButtonBox = new wxBoxSizer(wxHORIZONTAL);
	wxButton *ok_opt_button = new wxButton(this, id_ConfirmYes, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0);
	ButtonBox->Add(ok_opt_button, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	if (flag == 1) {
		wxButton *cancel_opt_button = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
		ButtonBox->Add(cancel_opt_button, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	}
	VRight->Add(ButtonBox, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	HMainSizer->Add(VRight, 1, wxEXPAND|wxALL, 5);

	SetSizerAndFit(HMainSizer);
}

CConfirmBox::~CConfirmBox(void)
{
}

//BEGIN_EVENT_TABLE(CConfirmBox, wxDialog)
//EVT_BUTTON(id_ConfirmYes, CAdmin::OnOK)
//END_EVENT_TABLE()
