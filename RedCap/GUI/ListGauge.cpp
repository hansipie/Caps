#include "general.h"

ListGauge::ListGauge(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos)
:wxDialog(parent, id, title, pos, wxSize(250, 430))
{
	MainFrame = (MyFrame *)parent;
	parent = this;



}