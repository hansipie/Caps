#include "general.h"


CExplorerClient::CExplorerClient(MyFrame *MainFrame, wxPanel *parent, int height, int xWin, int yWin, const wxString& bitmap_name)
{
	
#ifdef __WIN32__

	bitmap_left = new wxStaticBitmap(parent, -1,
						      	wxBitmap(bitmap_name, wxBITMAP_TYPE_BMP),
									wxPoint(0,0),
									wxDefaultSize
									);
	bitmap_left2 = new wxStaticBitmap(parent, -1,
									wxBitmap("./icons/client2.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(0,143),
									wxDefaultSize
									);
#else

       	bitmap_left = new wxStaticBitmap(parent, -1,
       				      	wxBitmap(bitmap_name, wxBITMAP_TYPE_BMP),
      								wxPoint(0,0),
	   							wxDefaultSize
									);
#endif

	right_page = new wxPanel(parent, -1, wxPoint(30, 0), wxSize(xWin-X_TRANSFER-190, 300));

	/**** on positionne les boutons d'actions en haut de chaque explorateur */

	wxBitmap ExploclientBitmaps[6];
	ExploclientBitmaps[0] = wxBitmap("./icons/create_folder.bmp", wxBITMAP_TYPE_BMP);
	ExploclientBitmaps[1] = wxBitmap("./icons/delete.bmp",wxBITMAP_TYPE_BMP);
	ExploclientBitmaps[2] = wxBitmap("./icons/reload.bmp",wxBITMAP_TYPE_BMP);
	ExploclientBitmaps[3] = wxBitmap("./icons/download.bmp",wxBITMAP_TYPE_BMP);
	ExploclientBitmaps[4] = wxBitmap("./icons/upload.bmp",wxBITMAP_TYPE_BMP);
	ExploclientBitmaps[5] = wxBitmap("./icons/delete.bmp",wxBITMAP_TYPE_BMP);

	wxMask *mask0 = new wxMask(ExploclientBitmaps[0], *wxWHITE);
    ExploclientBitmaps[0].SetMask(mask0);
	wxMask *mask1 = new wxMask(ExploclientBitmaps[1], *wxWHITE);
    ExploclientBitmaps[1].SetMask(mask1);
	wxMask *mask2 = new wxMask(ExploclientBitmaps[2], *wxWHITE);
	ExploclientBitmaps[2].SetMask(mask2);
	wxMask *mask3 = new wxMask(ExploclientBitmaps[3], *wxWHITE);
	ExploclientBitmaps[3].SetMask(mask3);
	wxMask *mask4 = new wxMask(ExploclientBitmaps[4], *wxWHITE);
	ExploclientBitmaps[4].SetMask(mask4);
	wxMask *mask5 = new wxMask(ExploclientBitmaps[5], *wxWHITE);
	ExploclientBitmaps[4].SetMask(mask5);


	create_folder_button = new wxBitmapButton(right_page, ID_CREATE_EXPLO_CLIENT,
								ExploclientBitmaps[0],
								wxPoint(X_TRANSFER - 30, 2),
								wxSize(25, 25));
	delete_button = new wxBitmapButton(right_page, ID_DELETE_EXPLO_CLIENT, 
								ExploclientBitmaps[1], 
								wxPoint(X_TRANSFER, 2),
								wxSize(25, 25));
	reload_button = new wxBitmapButton(	right_page, ID_RELOAD_EXPLO_CLIENT,
								ExploclientBitmaps[2],
								wxPoint(X_TRANSFER + 32, 2),
								wxSize(25, 25));
	
	download_button = new wxBitmapButton(right_page, ID_DOWNLOAD_EXPLO_CLIENT,
									ExploclientBitmaps[3],
									wxPoint(X_TRANSFER + (2 * 32), 2),
									wxSize(25, 25));
	upload_button = new wxBitmapButton(right_page, ID_UPLOAD_EXPLO_CLIENT,
									ExploclientBitmaps[4],
									wxPoint(X_TRANSFER + (3 * 32), 2),
									wxSize(25, 25));

	wxBitmap FileTransferBitmaps[2];
	FileTransferBitmaps[0] = wxBitmap("./icons/rep_server.bmp",wxBITMAP_TYPE_BMP);
	FileTransferBitmaps[1] = wxBitmap("./icons/file_server.bmp",wxBITMAP_TYPE_BMP);
	wxColor mask; 
	mask.Set(255,255,255);
	wxSize imageSize(16, 16);
    m_imageTreeCtrl   = new wxImageList( imageSize.GetWidth(), imageSize.GetHeight() );
    m_imageTreeCtrl->Add (FileTransferBitmaps[0], mask);
	m_imageTreeCtrl->Add (FileTransferBitmaps[1], mask);

	/************** On construit la page de droite **********/
	//right_page = new wxPanel(parent, -1, wxDefaultPosition, wxSize(350, height));
    tree_list = new wxListCtrl(right_page, 
										ID_LIST_EXPLO_CLIENT,
										wxPoint(0, 30), wxSize(xWin-X_TRANSFER-190, 100), 
										wxLC_REPORT|wxSUNKEN_BORDER );
	/* nom des colonnes (page de droite)*/

	tree_list->SetImageList(m_imageTreeCtrl, wxIMAGE_LIST_SMALL);
	
	tree_list->InsertColumn( 0, wxT("Name File"), wxLIST_FORMAT_LEFT, 110 );
    tree_list->InsertColumn( 1, wxT("Creator"), wxLIST_FORMAT_LEFT, 110 );
    tree_list->InsertColumn( 2, wxT("Size"), wxLIST_FORMAT_LEFT, 110 );
	tree_list->InsertColumn( 3, wxT("File Type"), wxLIST_FORMAT_LEFT, 110 );
}

CExplorerClient::~CExplorerClient(void)
{
}


void MyPanel::GetDraggedItemNameClient()
{
	wxMessageBox("You dragged my wife !?");
}

void MyPanel::CreateItemClient(wxCommandEvent& event)
{
	//wxTreeItemId item = panel_transfer->ExploClient->tree_transfer->GetSelection();
	//wxMessageBox("Create : " + item);
}
void MyPanel::DeleteItemClient(wxCommandEvent& event)
{
//	wxTreeItemId item = panel_transfer->ExploClient->tree_transfer->GetSelection();
//	wxString file_name = panel_transfer->ExploClient->tree_transfer->GetItemText(item);
	
//	if (file_name != "")
//	{
//		int answer_deleting = wxMessageBox("Do you really want to delete " + file_name + " ?", "File Transfer Information", wxICON_QUESTION | wxYES_NO );
//		if (answer_deleting == wxYES)
////			panel_transfer->ExploClient->tree_transfer->Delete(item);
//	}
//	else
//		wxMessageBox("No File to Delete have been selected", "File Transfer Information", wxICON_EXCLAMATION);
}
void MyPanel::ReloadExploClient(wxCommandEvent& event)
{
	//wxMessageBox("test");
	panel_transfer->ExploClient->tree_list->DeleteAllItems();
	MainFrame->output_message("lls");
}

void MyPanel::UploadFileClient(wxCommandEvent& event)
{
	wxString file_name;
	long item = panel_transfer->ExploClient->tree_list->GetNextItem(-1, wxLIST_NEXT_ALL,
                                        wxLIST_STATE_SELECTED);
	if (item != -1)
		file_name = panel_transfer->ExploClient->tree_list->GetItemText(item);
	if (file_name == "")
		wxMessageBox("You have to select a file to start the Upload on the server", "File Transfer Information", wxICON_EXCLAMATION);
	else
		MainFrame->output_message("upload \"" + file_name + "\"");
		//wxMessageBox("Dans UploadFileClient: UploadFile file [" + file_name + "]");
}

void MyPanel::OnDbleClickFileClient(wxTreeEvent& event)
{
//	wxTreeItemId item = panel_transfer->ExploClient->tree_transfer->GetSelection();
//	wxString file_name = panel_transfer->ExploClient->tree_transfer->GetItemText(item);
//	wxMessageBox("Open file [" + file_name + "]");
}

void MyPanel::GetFileInfo(wxCommandEvent& event)
{
	long item = panel_transfer->ExploServer->tree_list->GetNextItem(-1, wxLIST_NEXT_ALL,
                                        wxLIST_STATE_SELECTED);
	MainFrame->output_message("file_info \"" + panel_transfer->ExploServer->tree_list->GetItemText(item) + "\"");
}

void MyPanel::LaunchUpload(wxListEvent& event)
{
	long item = panel_transfer->ExploClient->tree_list->GetNextItem(-1, wxLIST_NEXT_ALL,
                                        wxLIST_STATE_SELECTED);
	wxMessageBox(panel_transfer->ExploClient->tree_list->GetItemText(item));
	MainFrame->output_message("upload \"" + panel_transfer->ExploClient->tree_list->GetItemText(item) + "\"");
	//output_message("upload " + panel_transfer->ExploClient->tree_list->GetItemText(item));
}

//void MyPanel::UplevelClient()
//{
//	output_message("lcd ..");
//}
