#include "general.h"

CExplorerServer::CExplorerServer(MyFrame *MainFrame, wxPanel *parent, int height, int xWin, int yWin, const wxString& bitmap_name)
{
	bitmap_left = new wxStaticBitmap(parent, -1,
									wxBitmap(bitmap_name, wxBITMAP_TYPE_BMP),
									wxPoint(0,0),
									wxDefaultSize
									);
	bitmap_left2 = new wxStaticBitmap(parent, -1,
									wxBitmap("./icons/server2.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(0,143),
									wxDefaultSize
									);
		
	/**** on positionne les boutons d'actions en haut de chaque explorateur */

	wxBitmap ExploserverBitmaps[5];
	ExploserverBitmaps[0] = wxBitmap("./icons/create_folder.bmp",wxBITMAP_TYPE_BMP);
	ExploserverBitmaps[1] = wxBitmap("./icons/delete.bmp",wxBITMAP_TYPE_BMP);
	ExploserverBitmaps[2] = wxBitmap("./icons/reload.bmp",wxBITMAP_TYPE_BMP);
	ExploserverBitmaps[3] = wxBitmap("./icons/up.bmp",wxBITMAP_TYPE_BMP);
	ExploserverBitmaps[4] = wxBitmap("./icons/file_info.bmp",wxBITMAP_TYPE_BMP);


	wxMask *mask0 = new wxMask(ExploserverBitmaps[0], *wxWHITE);
    ExploserverBitmaps[0].SetMask(mask0);
	wxMask *mask1 = new wxMask(ExploserverBitmaps[1], *wxWHITE);
    ExploserverBitmaps[1].SetMask(mask1);
	wxMask *mask2 = new wxMask(ExploserverBitmaps[2], *wxWHITE);
	ExploserverBitmaps[2].SetMask(mask2);
	wxMask *mask3 = new wxMask(ExploserverBitmaps[3], *wxWHITE);
	ExploserverBitmaps[3].SetMask(mask3);
	wxMask *mask4 = new wxMask(ExploserverBitmaps[4], *wxWHITE);
	ExploserverBitmaps[4].SetMask(mask4);

	create_folder_button = new wxBitmapButton(parent, ID_CREATE_EXPLO_SERVER,
								ExploserverBitmaps[0],
								wxPoint(X_TRANSFER, 5),
								wxSize(25, 25), wxBU_AUTODRAW);
	delete_button = new wxBitmapButton(parent, ID_DELETE_EXPLO_SERVER, 
								ExploserverBitmaps[1], 
								wxPoint(X_TRANSFER + 32, 5),
								wxSize(25, 25),	wxBU_AUTODRAW);
	reload_button = new wxBitmapButton(	parent, ID_RELOAD_EXPLO_SERVER,
								ExploserverBitmaps[2],
								wxPoint(X_TRANSFER + (2 * 32), 5),
								wxSize(25, 25), wxBU_AUTODRAW);
	preview_button = new wxBitmapButton(parent, ID_PREVIEW,
								wxBitmap("./icons/preview.bmp", wxBITMAP_TYPE_BMP),
								wxPoint(X_TRANSFER + (3 * 32), 5),
								wxSize(25, 25), wxBU_AUTODRAW);
	uplevel_button = new wxBitmapButton(parent, ID_UPLEVEL_EXPLO_SERVER,
									wxBitmap("./icons/up.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(X_TRANSFER + (4 * 32), 5),
									wxSize(25, 25));
	info_button = new wxBitmapButton(parent, ID_FILE_INFO_EXPLO_SERVER,
									wxBitmap("./icons/file_info.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(X_TRANSFER + (5 * 32), 5),
									wxSize(25, 25));

	//////////************ AJout de la barre de path *****************///////////////////
	PathBar = new wxTextCtrl(parent, -1, wxT(""), wxPoint(X_TRANSFER + (6 * 32), 5),  wxSize(350, 20), wxTE_READONLY | wxTE_RICH);

	//////////////////////////////////*******************///////
	wxBitmap FileTransferBitmaps[2];
	FileTransferBitmaps[0] = wxBitmap("./icons/rep_client.bmp",wxBITMAP_TYPE_BMP);
	FileTransferBitmaps[1] = wxBitmap("./icons/file_client.bmp",wxBITMAP_TYPE_BMP);
	wxColor mask;
	mask.Set(255,255,255);
	wxSize imageSize(16, 16);
    m_imageTreeCtrl   = new wxImageList( imageSize.GetWidth(), imageSize.GetHeight());
	
    m_imageTreeCtrl->Add (FileTransferBitmaps[0], mask);
    m_imageTreeCtrl->Add (FileTransferBitmaps[1], mask);
		/************** On construit la page de droite **********/
	
	right_page = new wxPanel(parent, -1, wxPoint(X_TRANSFER, Y_TRANSFER), wxSize(xWin-X_TRANSFER-190,100));

	tree_list = new wxListCtrl(right_page, 
										ID_LIST_EXPLO_SERVER,
										wxPoint(0, 0), wxSize(xWin-X_TRANSFER-190,100),
										/*wxLC_SORT_ASCENDING|*/wxLC_REPORT|wxSUNKEN_BORDER);

	tree_list->SetImageList(m_imageTreeCtrl, wxIMAGE_LIST_SMALL);
	
	///* nom des colonnes (page de droite)*/
	tree_list->InsertColumn( 0, wxT("Name File"), wxLIST_FORMAT_LEFT, 110 );
    tree_list->InsertColumn( 1, wxT("Creator"), wxLIST_FORMAT_LEFT, 110 );
    tree_list->InsertColumn( 2, wxT("Size"), wxLIST_FORMAT_LEFT, 110 );
	tree_list->InsertColumn( 3, wxT("File Type"), wxLIST_FORMAT_LEFT, 110 );
}

CExplorerServer::~CExplorerServer(void)
{
}

//void		MyPanel::OnRightLeftSashChangeServer(wxNotebookEvent &event)
//{
//	int sash;
//	sash = panel_transfer->SplitExplo->GetSashPosition();
//		/*************** resize de l'explorateur du server ******************/
//	int sash_server;
//	sash_server = panel_transfer->ExploServer->explo_window->GetSashPosition();
//	panel_transfer->SplitExplo->SetSize(x - WIDTH_UTILISATEUR - 10, y-50);
//	panel_transfer->ExploServer->explo_window->SetSize(x+2, sash - 10);
//	panel_transfer->ExploServer->left_page->SetSize(sash_server, sash-10);
//	panel_transfer->ExploServer->tree_transfer->SetSize(sash_server, sash-Y_TRANSFER);
//	panel_transfer->ExploServer->right_page->SetSize(x - WIDTH_UTILISATEUR - 10 - sash_server, sash);
//	panel_transfer->ExploServer->tree_list->SetSize(x - WIDTH_UTILISATEUR - 15 - sash_server-X_TRANSFER, sash-Y_TRANSFER);
//	panel_transfer->ExploServer->bitmap_left2->SetSize(0, 143, 27, sash);
//
//		/*************** resize de l'explorateur du client ******************/
//	int sash_client;
//	sash_client = panel_transfer->ExploClient->explo_window->GetSashPosition();
//	panel_transfer->ExploClient->explo_window->SetSize(x, y - sash - 98);
//	panel_transfer->ExploClient->left_page->SetSize(sash_client, y - sash - 60);
//	panel_transfer->ExploClient->tree_transfer->SetSize(sash_client, y - sash - 65 -Y_TRANSFER);
//	panel_transfer->ExploClient->tree_list->SetSize(x - WIDTH_UTILISATEUR - 18 - sash_client-X_TRANSFER, y  - sash - 65-Y_TRANSFER);
//	panel_transfer->ExploClient->bitmap_left2->SetSize(0, 143, 27, y - sash - 65 );
//}

void MyPanel::GetDraggedItemNameServer()
{

	wxMessageBox("You dragged my wife !?");

}

void MyPanel::CreateItemServer(wxCommandEvent& event)
{
	PopupCreate dialog_create_folder(this, -1, wxT("Folder Creation"), wxDefaultPosition);
//dialog_create_folder.
	dialog_create_folder.CentreOnParent();
	dialog_create_folder.ShowModal();
}

void MyPanel::DeleteItemServer(wxCommandEvent& event)
{
	long item = panel_transfer->ExploServer->tree_list->GetNextItem(-1, wxLIST_NEXT_ALL,
                                        wxLIST_STATE_SELECTED);
	wxString file_name;
	
	if (item != -1)
		file_name = panel_transfer->ExploServer->tree_list->GetItemText(item);
	if (file_name != "")
	{
		int answer_deleting = wxMessageBox("Do you really want to delete " + file_name + " ?", "File Transfer Information", wxICON_QUESTION | wxYES_NO );
		if (answer_deleting == wxYES)
		{
			panel_transfer->ExploServer->tree_list->DeleteItem(item);
			MainFrame->output_message("rm \"" + file_name + "\"");
			//wxMessageBox("rm " + file_name);
		}
	}
	else
		wxMessageBox("No File to Delete have been selected", "File Transfer Information", wxICON_EXCLAMATION);
}

void MyPanel::ReloadExploServer(wxCommandEvent& event)
{
	MainFrame->m_panel->panel_transfer->ExploServer->tree_list->DeleteAllItems();
	MainFrame->output_message("ls");
}

void MyPanel::DownloadFileServer(wxCommandEvent& event)
{
	wxString file_name;
	wxString cmd_download;
	
	/***** Verification, si c un rep on entre dedans, si c un fichie on lance le down***/
	wxListItem	info;

	long item = panel_transfer->ExploServer->tree_list->GetNextItem(-1, wxLIST_NEXT_ALL,
                                        wxLIST_STATE_SELECTED);
	if (item != -1)
		file_name = panel_transfer->ExploServer->tree_list->GetItemText(item);
	if (file_name == "")
		wxMessageBox("You have to select a file to start the Download", "File Transfer Information", wxICON_EXCLAMATION);
	else
	{
		info.m_itemId = item;
		info.m_col = 3;
		info.m_mask = wxLIST_MASK_TEXT;
		panel_transfer->ExploServer->tree_list->GetItem(info);
		
		if (!strncmp(info.m_text.c_str(), "Folder", 6))
		{
			FillFolder(file_name);
		}
		else // C'est un fichier
		{
			cmd_download = "download \"" + file_name + "\"";
			//wxMessageBox(cmd_download);
			MainFrame->output_message(cmd_download);
			/*panel_download->tree_list_download->InsertItem(0, file_name, -1);
			panel_download->tree_list_download->SetItem(0, 1, "10 / 105 Ko", -1);
			panel_download->tree_list_download->SetItem(0, 2, "32 ko/s", -1);
			panel_download->tree_list_download->SetItem(0, 3, "1 min 5 sec", -1);
			panel_download->tree_list_download->SetItem(0, 4, "0%", -1);*/
	//		wxMessageBox("Download file [" + file_name + "]");
		}
	}
}

void MyPanel::OnDbleClickFileServer(wxTreeEvent& event)
{
	wxMessageBox("onDbleClickFileServer !");
	/*long item = -1;
	wxListItem	info;

	item = m_panel->panel_transfert->tree_list->GetNextItem(item, wxLIST_NEXT_ALL,
																			wxLIST_STATE_SELECTED);
	if (item == -1)
		break;
	info.m_itemId = item;
	info.m_col = 3;
	info.m_mask = wxLIST_MASK_TEXT;
	m_panel->panel_transfert->tree_list->GetItem(info);
	wxMessageBox();*/
}

void MyPanel::LaunchDownload(wxListEvent& event)
{
	wxCommandEvent cmd;
	DownloadFileServer(cmd);
}

void MyPanel::UpLevelServer(wxCommandEvent& event)
{
	panel_transfer->ExploServer->tree_list->DeleteAllItems();
	MainFrame->CurrentId = wxTreeItemId();
	MainFrame->output_message("cd \"..\"");
	//MainFrame->output_message("pwd");
	MainFrame->output_message("ls");
}

void MyPanel::RefreshServerPath()
{
	MainFrame->output_message("pwd");
}
