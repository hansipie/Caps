#include "general.h"


CConfirmBox::CConfirmBox(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, wxString Message, wxWindowID id_ConfirmYes, int flag)
:wxDialog(parent, id, title, pos, wxSize(325, 135))
{
	parent = NULL;
	wxBitmap pic = wxBitmap("./icons/important.bmp",wxBITMAP_TYPE_BMP);
	wxMask *mask = new wxMask(pic, *wxWHITE);
    pic.SetMask(mask);
	wxStaticBitmap	*warning_bmp = new wxStaticBitmap(this, -1,
									pic,
									wxPoint(5,15),
									wxDefaultSize
									);
	wxStaticText *Message_label = new wxStaticText( this, ID_TEXT, Message, wxPoint(60, 30), wxDefaultSize, 0 );
	wxButton *ok_opt_button = new wxButton( this, id_ConfirmYes, wxT("Ok"), wxPoint(140, 70), wxDefaultSize,0);
	if (flag == 1)
		wxButton *cancel_opt_button = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxPoint(230, 70), wxDefaultSize,0);
}

CConfirmBox::~CConfirmBox(void)
{
}

//BEGIN_EVENT_TABLE(CConfirmBox, wxDialog)
//EVT_BUTTON(id_ConfirmYes, CAdmin::OnOK)
//END_EVENT_TABLE()