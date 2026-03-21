#include "general.h"

MyPanel::MyPanel( wxWindow * parent, int x, int y, int w, int h )
       : wxPanel( parent, -1, wxPoint(x, y), wxSize(w, h) )
{
	MainFrame = (MyFrame *)parent;
    m_notebook = NULL;
	GetClientSize(&x, &y);
	List_users = NULL;
	BitmapUser = new wxStaticBitmap(
									this, -1,
									wxBitmap("./icons/BitmapUser.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(2, 31),
									wxSize(WIDTH_UTILISATEUR - 2, 24)
									);
	List_users = new wxListCtrl(
									this,
									ID_LISTBOX_UTILISATEUR,
									wxPoint(0, 130),
									wxSize(WIDTH_UTILISATEUR,230),
									wxSUNKEN_BORDER | /*wxLC_SMALL_ICON |*/ wxLC_LIST);
	
	List_serv = new wxComboBox(this, LIST_SERVER, _T(""), wxPoint(2,5), wxSize(168,-1) );

    if (0)
        wxLog::AddTraceMask(_T("focus"));

	wxSize imageSize(22, 22);
	wxBitmap toolBarBitmaps[20];
	toolBarBitmaps[0] = wxBitmap("./icons/chatpublic.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[1] = wxBitmap("./icons/transfert.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[2] = wxBitmap("./icons/download22.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[3] = wxBitmap("./icons/privchat.bmp",wxBITMAP_TYPE_BMP);

	toolBarBitmaps[4] = wxBitmap("./icons/niv1.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[5] = wxBitmap("./icons/niv1a.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[6] = wxBitmap("./icons/niv1r.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[7] = wxBitmap("./icons/niv1ar.bmp",wxBITMAP_TYPE_BMP);

	toolBarBitmaps[8] = wxBitmap("./icons/niv2.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[9] = wxBitmap("./icons/niv2a.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[10] = wxBitmap("./icons/niv2r.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[11] = wxBitmap("./icons/niv2ar.bmp",wxBITMAP_TYPE_BMP);

	toolBarBitmaps[12] = wxBitmap("./icons/niv3.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[13] = wxBitmap("./icons/niv3a.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[14] = wxBitmap("./icons/niv3r.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[15] = wxBitmap("./icons/niv3ar.bmp",wxBITMAP_TYPE_BMP);

	toolBarBitmaps[16] = wxBitmap("./icons/niv4.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[17] = wxBitmap("./icons/niv4a.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[18] = wxBitmap("./icons/niv4r.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[19] = wxBitmap("./icons/niv4ar.bmp",wxBITMAP_TYPE_BMP);

	wxColor mask; 
	
	mask.Set(255,255,255);

    m_imageList   = new wxImageList( imageSize.GetWidth(), imageSize.GetHeight() );

	for (int i = 0; i < 19; i++)
	{
		m_imageList->Add (toolBarBitmaps[i], mask);
    }

	List_users->SetImageList(m_imageList, wxIMAGE_LIST_SMALL);

	m_notebook = new wxNotebook(this, ID_NOTEBOOK);
	m_notebook->SetImageList(m_imageList);
/************** Onglet Chat Publique *****************/

	panel_public = new CPanelPublic(this, x, y);

/*****************************************************/

	panel_transfer = new CTranfert(MainFrame, m_notebook, x, y); // On cree la classe transfert

/***************** Onglet Download ************************************/

	panel_download = new CDownload(/*this,*/ m_notebook, x, y);

/***************** Onglet NewsGroup ************************************/

	//panel_news  = new CNewsGroup(m_notebook, x, y);

	Current_serv_data = NULL;
	Serv_Data.clear();
	MainFrame->connect_menu->Enable(CONTROLS_DISCONNECT, FALSE);
}

void MyPanel::SendChat(wxCommandEvent & event)
{
	wxString msg;
	wxColour login_colour;

	login_colour.Set(148,166,244);

	if (m_notebook->GetSelection() == 0)
	{
		if (MainFrame->opt_crypto) // if crypto enabled
		{
			wxMessageBox("Je crypte");
			msg = msg + "send_chat \"";
			msg = msg + MainFrame->crypto( panel_public->ChatText->GetValue() , MainFrame->Pass_crypto, 'c')+ "\"";
		}
		else
		{
			msg = "send_chat \"" + panel_public->ChatText->GetValue() + "\"";
		}
		//msg = "send_chat \"" + wxT(panel_public->ChatText->GetValue()) + "\"";
		MainFrame->output_message (msg);
		panel_public->ChatText->Clear();
	}
	else
	{
		CServData					*dataserv;
		CUser						*user = NULL;
		CPrivChat					*tmp;
		char						*buf;
		//int							serv;
		
		tmp = List_PrivChat[m_notebook->GetSelection() - nb_tab];
		tmp->TopChatWindow->SetFont(wxFont(8, wxDECORATIVE, wxNORMAL, wxBOLD, FALSE, "Unicode"));
		
		tmp->TopChatWindow->SetDefaultStyle(wxTextAttr(login_colour));
		tmp->TopChatWindow->AppendText(wxString(MainFrame->Name) + " says : " + "\n");
		tmp->TopChatWindow->SetDefaultStyle(wxTextAttr(*wxBLACK));
		tmp->TopChatWindow->AppendText(tmp->ChatText->GetValue() + "\n");

		dataserv = Current_serv_data;
		for (vector<CUser * >::iterator i = dataserv->Users.begin(); i != dataserv->Users.end(); ++i)
		{
			user = *i;
			if (!wxStrcmp(user->name, tmp->name))
				break;
		}

		msg = "send_priv_chat ";

		buf = new char [6];
		sprintf(buf, "%d", user->id);
	
		if (MainFrame->opt_crypto) // if crypto enabled
		{
			wxMessageBox("Je crypte");
			msg = msg + buf + " \"" + MainFrame->crypto( tmp->ChatText->GetValue() , MainFrame->Pass_crypto, 'c')+ "\"";
		}
		else
		{
			msg = msg + buf + " \"" + tmp->ChatText->GetValue() + "\"";
		}

		MainFrame->output_message(msg);
		tmp->ChatText->Clear();
	}	
}


void MyPanel::OnSize(wxSizeEvent& WXUNUSED(event))
{
    x = 0;
    y = 0;

    GetClientSize( &x, &y );
	MainFrame->degrade_title->SetSize(x - 350, 33);
	if (m_notebook)
		m_notebook->SetSize( WIDTH_UTILISATEUR + 2, 2, x-4 - WIDTH_UTILISATEUR, y - 2);
	if (List_users) 
		List_users->SetSize(2, 57 /*32*/, WIDTH_UTILISATEUR - 2, y - 58 /*32*/ );
	/* Resize de la barre de titre*/
	if (panel_public->splitter)
	{
#ifdef __WIN32__

		panel_public->BitmapDegr->SetSize(160, 0, x - 342, 24);
#endif
		panel_public->splitter->SetSize(2, 26, x - 13 - WIDTH_UTILISATEUR, y - 64);
		panel_public->splitter->SetSashPosition(y - 109);
		panel_public->y_sash = panel_public->splitter->GetSashPosition();
		panel_public->SendChatButton->SetSize(x - 275, (y  - panel_public->y_sash - 67) / 2 - 24, 86, 49 );
		panel_public->ChatText->SetSize(0, 0, x  - 113 - WIDTH_UTILISATEUR, y  - panel_public->y_sash - 67);
	}
	
	/** Resize de l'onglet TRANSFER de Fichier **/
	//panel_transfer->BitmapDegr->SetSize(160, 0, x - 342, 24);
	int sash;
	sash = panel_transfer->SplitExplo->GetSashPosition();
#ifdef __WIN32__
	panel_transfer->BitmapDegr->SetSize(160, 0, x - 342, 24);
#endif
	/*************** resize de l'explorateur du server ******************/
	panel_transfer->SplitExplo->SetSize(x - WIDTH_UTILISATEUR - 10, y-20);
	panel_transfer->ExploServer->right_page->SetSize(x - WIDTH_UTILISATEUR - 38, sash-35);
	panel_transfer->ExploServer->tree_list->SetSize(x - WIDTH_UTILISATEUR - 42, sash-35);

#ifdef __WIN32__
	panel_transfer->ExploServer->bitmap_left2->SetSize(0, 143, 27, sash);
#endif
		/*************** resize de l'explorateur du client ******************/
	panel_transfer->ExploClient->right_page->SetSize(x - WIDTH_UTILISATEUR - 38, y - sash);
	panel_transfer->ExploClient->tree_list->SetSize(x - WIDTH_UTILISATEUR - 42, y - sash - 95);

#ifdef __WIN32__	
	panel_transfer->ExploClient->bitmap_left2->SetSize(0, 143, 27, y - sash - 100 );
#endif
				/** resize de l'onglet DOWNLOAD de fichier **/
	panel_download->tree_list_download->SetSize(2, 59, x - (WIDTH_UTILISATEUR + 14), y - 100);
#ifdef __WIN32__
	panel_download->BitmapDegr->SetSize(160, 0, x - 342, 24);
#endif
				/** resize de l'onglet NEWSGROUP			**/
	//panel_news->tree_list_download->SetSize(2, 26, x - (WIDTH_UTILISATEUR + 16), y - 62);
	//panel_news->BitmapDegr->SetSize(160, 0, x - 342, 24);

	/*********** resize des PRIVATE CHAT ********/
	vector< CPrivChat * >::iterator i = List_PrivChat.begin();
	for ( /*vector< CPrivChat * >::iterator i = list_PrivChat.begin()*/; i != List_PrivChat.end(); ++i )
	{
		CPrivChat  *tmp = *i;
		//tmp->splitter;
		if (tmp->splitter)
		{

			tmp->BitmapDegr->SetSize(160,0, x - 365, 24);
			tmp->splitter->SetSize(2, 26, x - 13 - WIDTH_UTILISATEUR, y - 64);
			tmp->splitter->SetSashPosition(y - 115);
			tmp->y_sash = tmp->splitter->GetSashPosition();
			tmp->SendChatButton->SetSize(x - 275, (y  - tmp->y_sash - 67) / 2 - 24, 86, 49 );
			tmp->CloseButton->SetSize(x - WIDTH_UTILISATEUR - 35, 0, 24, 24);
			tmp->ChatText->SetSize(0, 0, x - 113 - WIDTH_UTILISATEUR, y  - tmp->y_sash - 67);
		}
	}
}
void MyPanel::OnPageChanged(wxNotebookEvent &event)
{
	Selected_page = m_notebook->GetSelection();
	
if (Selected_page == 2)
{
		//wxMessageBox("Page 3");
		
			MainFrame->output_message("task");
}
	else if (Selected_page == -1)
		wxMessageBox("Page -1");
}

void MyPanel::OnChatSashChange(wxSplitterEvent &event)
{
	int x = 0 ;
	int y = 0 ; 
	panel_public->y_sash = 0;
	
	GetClientSize( &x, &y );
	panel_public->y_sash = panel_public->splitter->GetSashPosition();
	panel_public->SendChatButton->SetSize(x - 275, (y  - panel_public->y_sash - 67) / 2 - 24, 86, 49 );
	panel_public->ChatText->SetSize(0, 0, x - 113 - WIDTH_UTILISATEUR, y  - panel_public->y_sash - 67);
	for ( vector< CPrivChat * >::iterator it = List_PrivChat.begin(); it != List_PrivChat.end(); ++it)
	{
		CPrivChat  *tmp = *it;
		tmp->y_sash = tmp->splitter->GetSashPosition();
		tmp->SendChatButton->SetSize(x - 275, (y  - tmp->y_sash - 67) / 2 - 24, 86, 49 );
		tmp->ChatText->SetSize(0, 0, x - 113 - WIDTH_UTILISATEUR, y  - tmp->y_sash - 67);
	}
}



void MyPanel::OnTransferSashChange(wxSplitterEvent &event)
{
	int sash;

	sash = panel_transfer->SplitExplo->GetSashPosition();
		/*************** resize de l'explorateur du server ******************/
	panel_transfer->ExploServer->right_page->SetSize(x - 213, sash-35);
	panel_transfer->ExploServer->tree_list->SetSize(x - 213, sash-35);

		/*************** resize de l'explorateur du client ******************/
	panel_transfer->ExploClient->right_page->SetSize(x - 213, y - sash);
	panel_transfer->ExploClient->tree_list->SetSize(x - 213, y - sash-95);
#ifdef __WIN32__
	panel_transfer->ExploServer->bitmap_left2->SetSize(0, 143, 27, sash);
	panel_transfer->ExploClient->bitmap_left2->SetSize(0, 143, 27, y - sash - 100 );
#endif
}

MyPanel::~MyPanel()
{
	//delete wxLog::SetActiveTarget(m_logTargetOld);
    delete m_notebook->GetImageList();
}

void MyPanel::suppr_PrivChat( wxCommandEvent &event )
{
	int sel = m_notebook->GetSelection();
	int n;

	m_notebook->DeletePage(sel);
	n = nb_tab;
	vector<CPrivChat *>::iterator it = List_PrivChat.begin();
	for (; it != List_PrivChat.end() && n != sel; ++it, n++)
		;
	List_PrivChat.erase(it);
}
