#pragma once

class CConfirmBox: public wxDialog
{
public:
	CConfirmBox(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, wxString Message, wxWindowID id_ConfirmYes, int flag);
	virtual ~CConfirmBox(void);
};
