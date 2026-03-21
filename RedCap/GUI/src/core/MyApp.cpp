#include "general.h"

//----------------------------------------------------------------------
// MyApp
//----------------------------------------------------------------------

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    // use standard command line handling:
	
	if ( !wxApp::OnInit())
        return FALSE;
    MyFrame *frame = new MyFrame(_T("Interface Caps"), 150, 100);
    new wxSplashScreen(wxBitmap("./icons/splash.bmp", wxBITMAP_TYPE_BMP), 
					 wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT, 
					1000, frame,
					-1, wxDefaultPosition, 
					wxDefaultSize, 
					wxSIMPLE_BORDER|wxFRAME_NO_TASKBAR|wxSTAY_ON_TOP);
					wxSleep(1);
	frame->Show(TRUE);
	wxCommandEvent event;
	frame->OnConnect_client(event);

    return TRUE;
}

//----------------------------------------------------------------------
// MyPanel
//----------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyPanel, wxPanel)
EVT_SIZE      (MyPanel::OnSize)
EVT_NOTEBOOK_PAGE_CHANGED(ID_NOTEBOOK,  MyPanel::OnPageChanged)
EVT_SPLITTER_SASH_POS_CHANGED(ID_CHAT_SPLITTER, MyPanel::OnChatSashChange)
EVT_BUTTON(ID_SEND_PUBLIC_CHAT, MyPanel::SendChat)
EVT_BUTTON(ID_CLOSE_CHAT, MyPanel::suppr_PrivChat)
EVT_LIST_ITEM_ACTIVATED(ID_LISTBOX_UTILISATEUR, MyPanel::OnDbleClickUtilisateur)

EVT_SPLITTER_SASH_POS_CHANGED(ID_SPLIT_EXPLO, MyPanel::OnTransferSashChange)


/* Fonction de gestion du serveur de fichier */
EVT_BUTTON(ID_DELETE_EXPLO_SERVER, MyPanel::DeleteItemServer)
EVT_BUTTON(ID_CREATE_EXPLO_SERVER, MyPanel::CreateItemServer)
EVT_BUTTON(ID_RELOAD_EXPLO_SERVER, MyPanel::ReloadExploServer)
EVT_LIST_ITEM_ACTIVATED(ID_LIST_EXPLO_SERVER, MyPanel::LaunchDownload)
EVT_TREE_ITEM_ACTIVATED(ID_TREE_EXPLO_SERVER, MyPanel::OnDbleClickFileServer)
EVT_BUTTON(ID_UPLEVEL_EXPLO_SERVER, MyPanel::UpLevelServer)
EVT_BUTTON(ID_FILE_INFO_EXPLO_SERVER, MyPanel::GetFileInfo)


/* Appel des fonctions de gestion de preview*/
EVT_BUTTON(ID_PREVIEW, MyPanel::PreviewFile)
EVT_LIST_ITEM_SELECTED(ID_LIST_EXPLO_SERVER, MyPanel::CheckPrevExtension)

/* Fonction de gestion des fichiers du client de fichier */
EVT_BUTTON(ID_DELETE_EXPLO_CLIENT, MyPanel::DeleteItemClient)
EVT_BUTTON(ID_CREATE_EXPLO_CLIENT, MyPanel::CreateItemClient)
EVT_BUTTON(ID_RELOAD_EXPLO_CLIENT, MyPanel::ReloadExploClient)
EVT_BUTTON(ID_UPLOAD_EXPLO_CLIENT,  MyPanel::UploadFileClient)
EVT_BUTTON(ID_DOWNLOAD_EXPLO_CLIENT, MyPanel::DownloadFileServer)
EVT_LIST_ITEM_ACTIVATED(ID_LIST_EXPLO_CLIENT, MyPanel::LaunchUpload)
EVT_TREE_ITEM_ACTIVATED(ID_TREE_EXPLO_CLIENT, MyPanel::OnDbleClickFileClient)


/* Fonction de gestion des fichiers du panneau Task */
EVT_LIST_ITEM_SELECTED(ID_LIST_DOWNLOAD, MyPanel::OnSelected)
EVT_BUTTON(ID_PLAY_FILE, MyPanel::play_file)
EVT_BUTTON(ID_PAUSE_FILE, MyPanel::pause_file)
EVT_BUTTON(ID_ERASE_FILE, MyPanel::erase_file)
END_EVENT_TABLE()

//----------------------------------------------------------------------
// CPrivChat
//----------------------------------------------------------------------
BEGIN_EVENT_TABLE(CPrivChat, wxPanel)
END_EVENT_TABLE()

//----------------------------------------------------------------------
// MyFrame
//----------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(CONTROLS_QUIT,   MyFrame::OnQuit)
	EVT_MENU(CONTROLS_OPTIONS,   MyFrame::OnOptions)
	EVT_MENU(CONTROLS_ADMIN,   MyFrame::OnAdmin)
    EVT_MENU(CONTROLS_ABOUT,  MyFrame::OnAbout)
	EVT_MENU(CONTROLS_BROADCAST,  MyFrame::OnSendBroadcast)
    EVT_MENU(CONTROLS_CLEAR_LOG,  MyFrame::OnClearLog)
	EVT_MENU(CONTROLS_CONNECT,  MyFrame::OnConnect)
	EVT_MENU(CONTROLS_DISCONNECT,  MyFrame::OnDisconnect)
	EVT_MENU(CONTROLS_CONNECT_CLIENT,  MyFrame::OnConnect_client)
	EVT_SOCKET(SOCKET_ID,     MyFrame::OnSocketEvent)
    EVT_SIZE(MyFrame::OnSize)
    EVT_MOVE(MyFrame::OnMove)
	EVT_TEXT(LIST_SERVER, MyFrame::change_serv)
	//lEVT_MENU(MyFrame::OnFireCustom)
    EVT_IDLE(MyFrame::OnIdle)
END_EVENT_TABLE()

//----------------------------------------------------------------------
// BroadcastDialog
//----------------------------------------------------------------------

BEGIN_EVENT_TABLE(BroadcastDialog, wxDialog)
EVT_BUTTON(ID_SEND_BROADCAST, BroadcastDialog::OnSendBroadcast)
END_EVENT_TABLE()
