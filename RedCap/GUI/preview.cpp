#include "general.h"
#include "wx/image.h" 
#define NOM "./preview.jpg"


void MyFrame::IntToBox(int i)
{
	char *tmp = new char[25];
	sprintf(tmp, "%d", i);
	wxMessageBox(tmp);
}

void MyFrame::preview_in_file(char *stream, int bit_left)
{


	if (!preview_curr_size)
	{	
		preview_file.Open(NOM, wxFile::write);

		//////////////////////////////////////////////////////////////////////////////
		//m_panel->panel_public->TopChatWindow->SetDefaultStyle(wxTextAttr(*wxBLACK));
		//m_panel->panel_public->TopChatWindow->AppendText("fichier ouvert\n");
		//////////////////////////////////////////////////////////////////////////////
	}

	preview_curr_size = preview_curr_size + bit_left;
	if (preview_curr_size > preview_size)
	{
		bit_left = bit_left - (preview_curr_size - preview_size);
		//////////////////////////////////////////////////////////////////////////////
		//m_panel->panel_public->TopChatWindow->SetDefaultStyle(wxTextAttr(*wxBLACK));
		//m_panel->panel_public->TopChatWindow->AppendText("detection de fin de fichier\n");
		//////////////////////////////////////////////////////////////////////////////
	}

	//////////////////////////////////////////////////////////////////////////////
	//m_panel->panel_public->TopChatWindow->SetDefaultStyle(wxTextAttr(*wxBLACK));
	//char *zzz = new char[35];
	//sprintf(zzz, "\n____j'ecris_%d____\n", bit_left);
	//m_panel->panel_public->TopChatWindow->AppendText(zzz);
	//////////////////////////////////////////////////////////////////////////////

	preview_file.Write(stream, bit_left);
	bit_left = 0;

	if (preview_curr_size >= preview_size)
	{
		preview_file.Close();
		preview_file.Close();
		preview_file.Close();
		preview_curr_size = 0;
		preview_size = 0; 

		//////////////////////////////////////////////////////////////////////////////
		//m_panel->panel_public->TopChatWindow->SetDefaultStyle(wxTextAttr(*wxBLACK));
		//m_panel->panel_public->TopChatWindow->AppendText("finalisation du fichier\n");
		//////////////////////////////////////////////////////////////////////////////

		wxImage::AddHandler( new wxJPEGHandler );
		wxImage image;	
		if (image.LoadFile(NOM, wxBITMAP_TYPE_JPEG))
		{
			wxBitmap bitmap(image, 16);
			int	height = bitmap.GetHeight();
			int width = bitmap.GetWidth();

			/* on ouvre la fenetre de preview */		
			wxFrame *prev = new wxFrame(this, -1, "Preview", wxPoint(500, 400), wxSize(width, height+20));
			new wxStaticBitmap(prev, -1, bitmap, wxPoint(0,0), wxDefaultSize);
			prev->Show(TRUE);
		}
	}

	return ;
}

void MyPanel::CheckPrevExtension(int id)
{
	long item = -1;
	int fl = -1;

	for ( ; fl == -1; )
    {
        item = this->panel_transfer->ExploServer->tree_list->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( item == -1 )
            break;
		fl = 1;		
        // this item is selected - do whatever is needed with it
    }
	wxString file_name = this->panel_transfer->ExploServer->tree_list->GetItemText(item);
	wxString extension = file_name.AfterLast('.');
	if (extension.CmpNoCase("pdf") || extension.CmpNoCase("mpeg") || 
			extension.CmpNoCase("txt") || extension.CmpNoCase("mpg") || 
			extension.CmpNoCase("jpg")  || extension.CmpNoCase("bmp") ||
			extension.CmpNoCase("jpeg")  || extension.CmpNoCase("png") ||
			extension.CmpNoCase("gif"))
	{
		;
		//wxMessageBox(file_name);

		//panel_transfer->ExploServer->preview_button.SetBitmapDisabled();
		//SetBitmapDisabled(panel_transfer->ExploServer->preview_button);
		//panel_transfer->ExploServer->preview_button->
		//SetBitmapDisabled();
	}
	else
		wxMessageBox("Pas de preview sur ce type de fichier");
}

void MyPanel::PreviewFile()
{
	long item = -1;
	int fl = -1;

	//MainFrame->Affstruct(); return;
	for ( ; fl == -1; )
    {
        item = this->panel_transfer->ExploServer->tree_list->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( item == -1 )
            break;
		fl = 1;
        // this item is selected - do whatever is needed with it
    }
	wxString file_name = this->panel_transfer->ExploServer->tree_list->GetItemText(item);
	//wxMessageBox("Lancement du preview de " +  file_name + ".");
	MainFrame->output_message("preview \"" + file_name + "\"");
}
