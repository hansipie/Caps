#include "general.h"

CDownload::CDownload(wxNotebook *m_notebook, int xWin,int yWin)
{
	panel_download = new wxPanel(m_notebook);

	wxBitmap TaskBitmaps[3];
	TaskBitmaps[0] = wxBitmap("./icons/resume_file.bmp",wxBITMAP_TYPE_BMP);
	TaskBitmaps[1] = wxBitmap("./icons/pause_file.bmp",wxBITMAP_TYPE_BMP);
	TaskBitmaps[2] = wxBitmap("./icons/erase_file.bmp",wxBITMAP_TYPE_BMP);
	
	wxMask *mask0 = new wxMask(TaskBitmaps[0], *wxWHITE);
    TaskBitmaps[0].SetMask(mask0);
	wxMask *mask1 = new wxMask(TaskBitmaps[1], *wxWHITE);
    TaskBitmaps[1].SetMask(mask1);
	wxMask *mask2 = new wxMask(TaskBitmaps[2], *wxWHITE);
	TaskBitmaps[2].SetMask(mask2);

#ifdef __WIN32__
	BitmapDownload = new wxStaticBitmap(
									panel_download, -1,
									wxBitmap("./icons/BitmapDownload.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(0, 0),
									wxSize(160, 24)
									);
	BitmapDegr = new wxStaticBitmap(
									panel_download, -1,
									wxBitmap("./icons/BitmapDegr.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(160, 0),
									wxSize(xWin + 10 - 160, 24)
									);
#else
	BitmapDownload = new wxStaticBitmap(
									panel_download, -1,
									wxBitmap("./icons/download_unix.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(0, 0),
									wxSize(310, 24)
									);
#endif

	resume_file =	new wxBitmapButton(panel_download, ID_PLAY_FILE,
								TaskBitmaps[0],
								wxPoint(2, 28),
								wxSize(25, 25), wxBU_AUTODRAW);

	pause_file = new wxBitmapButton(panel_download, ID_PAUSE_FILE,
								TaskBitmaps[1],
								wxPoint(2 + 28, 28),
								wxSize(25, 25), wxBU_AUTODRAW);

	erase_file = new wxBitmapButton(panel_download, ID_ERASE_FILE,
								TaskBitmaps[2],
								wxPoint(2 + (2 * 28), 28),
								wxSize(25, 25), wxBU_AUTODRAW);

	/*m_imageTask->Add (FileTransferBitmaps[0], mask);
	m_imageTask->Add (FileTransferBitmaps[1], mask);
	tree_transfer->SetImageList(m_imageTreeCtrl);*/

	tree_list_download = new wxListCtrl(panel_download,
										ID_LIST_DOWNLOAD,
										wxPoint(102, 50), wxDefaultSize, 
										wxLC_REPORT|wxSUNKEN_BORDER );

	tree_list_download->InsertColumn( 0, wxT("Type"), wxLIST_FORMAT_LEFT, 50 );
    tree_list_download->InsertColumn( 1, wxT("File"), wxLIST_FORMAT_LEFT, 200 );
	tree_list_download->InsertColumn( 2, wxT("Transfert"), wxLIST_FORMAT_LEFT, 60 );
	tree_list_download->InsertColumn( 3, wxT("Size"), wxLIST_FORMAT_LEFT, 60 );
	tree_list_download->InsertColumn( 4, wxT("Progress"), wxLIST_FORMAT_LEFT, 60 );
	tree_list_download->InsertColumn( 5, wxT("Speed"), wxLIST_FORMAT_LEFT, 60 );
	tree_list_download->InsertColumn( 6, wxT("Status"), wxLIST_FORMAT_LEFT, 100 );
	tree_list_download->InsertColumn( 7, wxT("Reference"), wxLIST_FORMAT_LEFT, 100 );
	
	m_notebook->AddPage(panel_download, _T("Transfer"), FALSE, 2);
	task_selected_ref = "";
	task_selected_file = "";
}

void MyPanel::OnSelected(wxListEvent& event)
{
	//wxListCtrl *mylist = panel_download->tree_list_download;

////	long item = mylist->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	//wxListItem info;
	//wxString ref;
	//wxString fich;

 //   info.m_itemId = event.m_itemIndex;
 //   info.m_mask = wxLIST_MASK_TEXT;
	//info.m_col = 1;
	//mylist->GetItem(info);
	//fich = info.m_text.c_str();
	//mylist->GetItem(info);
 //   info.m_col = 7;
	//ref = info.m_text.c_str();

	//if ( mylist->GetItem(info))
	//{
	//	panel_download->task_selected_file = fich;
	//	panel_download->task_selected_ref = ref;
	//}
	//else
 //   {
	//	wxFAIL_MSG(wxT("wxListCtrl::GetItem() failed"));
 //   }
}


void MyPanel::play_file(wxListEvent& event)
{
	long item = -1;
	int flag = 0;
	wxListItem	info;

	for (;;)
	{
		item = panel_download->tree_list_download->GetNextItem(item,
															wxLIST_NEXT_ALL,
															wxLIST_STATE_SELECTED);
		if (item == -1)
			break;
		flag = 1;
		info.m_itemId = item;
		info.m_col = 1;
		info.m_mask = wxLIST_MASK_TEXT;
		panel_download->tree_list_download->GetItem(info);
		//wxMessageBox("lol");

		//wxMessageBox("download \"" + wxString(info.m_text.c_str()) + "\"");
		MainFrame->output_message("download \"" + wxString(info.m_text.c_str()) + "\"");
		MainFrame->output_message("ls");
	}
	if (flag == 0)
		wxMessageBox("You have to select a file to start the Download", "Task Information", wxICON_EXCLAMATION);
}

void MyPanel::pause_file()
{
	long item = -1;
	int flag = 0;
	wxListItem	info;

	for (;;)
	{
		item = panel_download->tree_list_download->GetNextItem(item,
															wxLIST_NEXT_ALL,
															wxLIST_STATE_SELECTED);
		if (item == -1)
			break;
		flag = 1;
		info.m_itemId = item;
		info.m_col = 7;
		info.m_mask = wxLIST_MASK_TEXT;
		panel_download->tree_list_download->GetItem(info);

		//wxMessageBox("stop_task \"" + wxString(info.m_text.c_str()) + "\"");
		MainFrame->output_message("stop_task \"" + wxString(info.m_text.c_str()) + "\"");
	}
	if (flag == 0)
		wxMessageBox("You have to select a file to pause the Download", "Task Information", wxICON_EXCLAMATION);

}

void MyPanel::erase_file()
{
	long item = -1;
	int flag = 0;
	wxListItem	info;

	for (;;)
	{
		item = panel_download->tree_list_download->GetNextItem(item,
															wxLIST_NEXT_ALL,
															wxLIST_STATE_SELECTED);
		if (item == -1)
			break;
		flag = 1;
		info.m_itemId = item;
		info.m_col = 7;
		info.m_mask = wxLIST_MASK_TEXT;
		panel_download->tree_list_download->GetItem(info);

		//wxMessageBox("delete_task \"" + wxString(info.m_text.c_str()) + "\"");
		MainFrame->output_message("delete_task " + wxString(info.m_text.c_str()));
	}
	if (flag == 0)
		wxMessageBox("You have to select a file to erase the Download from the list", "Task Information", wxICON_EXCLAMATION);
	MainFrame->output_message("tasks");
}

CDownload::~CDownload(void)
{
}
