#define WIDTH_UTILISATEUR 170
#define X_TRANSFER 30
#define Y_TRANSFER 34

enum
{
	ID_NOTEBOOK=   wxID_HIGHEST + 1,
	ID_LISTBOX,
	ID_LISTBOX_UTILISATEUR,
	ID_TEXTCTRL,
	ID_BUTTON,
	ID_SEND_PUBLIC_CHAT,
	ID_CLOSE_CHAT,
	ID_FILE_SPLITTER,
	ID_CHAT_SPLITTER,
	ID_LIST_DOWNLOAD,
	ID_SPLIT_EXPLO,

	ID_LIST_EXPLO_SERVER,
	ID_TREE_EXPLO_SERVER,
	ID_DELETE_EXPLO_SERVER,
	ID_CREATE_EXPLO_SERVER,
	ID_RELOAD_EXPLO_SERVER,
	ID_DOWNLOAD_EXPLO_SERVER,
	ID_UPLOAD_EXPLO_SERVER,
	/*Rajout du bouton de preview pour lancer la fenetre de visualisation*/
	ID_PREVIEW,
	/*************************************/
	ID_LIST_EXPLO_CLIENT,
	ID_TREE_EXPLO_CLIENT,
	ID_RIGHT_LEFT_SPLITTER_CLIENT,
	ID_DELETE_EXPLO_CLIENT,
	ID_CREATE_EXPLO_CLIENT,
	ID_RELOAD_EXPLO_CLIENT,
	ID_DOWNLOAD_EXPLO_CLIENT,
	ID_UPLOAD_EXPLO_CLIENT,
	ID_UPLEVEL_EXPLO_CLIENT,
	ID_UPLEVEL_EXPLO_SERVER,
	ID_FILE_INFO_EXPLO_SERVER, // Ajout du file info
	ID_PLAY_FILE,
	ID_ERASE_FILE,
	ID_PAUSE_FILE
};

class CPrivChat;
class CServData;
class CTranfert;
class CAgree;
class CPanelPublic;
class CDownload;
class CExplorerServer;
class CExplorerClient;
class CUser;
class CNewsGroup;

class MyPanel: public wxPanel
{
public:
    MyPanel(wxWindow *frame, int x, int y, int w, int h);
    virtual ~MyPanel();
    MyFrame					*MainFrame;
    wxNotebook				*m_notebook;
	wxImageList				*m_imageList;
	vector<CServData *>		Serv_Data;
	CServData				*Current_serv_data;
	wxStaticBitmap			*BitmapUser;
	wxListCtrl				*List_users;
	int						Selected_page;
	
	wxComboBox				*List_serv;

	vector<CPrivChat *>		List_PrivChat;

	CPanelPublic			*panel_public;
	CTranfert				*panel_transfer;
	CDownload				*panel_download;
	CNewsGroup				*panel_news;

	const static int		nb_tab = 3; 
	/** Coord fenetre **/
	int x, y;
	
	void PreviewFile();
	void CheckPrevExtension(int id);
    void OnSize( wxSizeEvent& event );
    void OnPageChanged( wxNotebookEvent &event );
	void OnChatSashChange(wxNotebookEvent &event);
	void SendChat(wxCommandEvent & event);
//	void CheckKeys(wxKeyEvent& event);
	void suppr_PrivChat(wxCommandEvent& event);
	//void PanelPublicChat();
	void OnDbleClickUtilisateur();
	void OnFileSashChange();
	
	/* Fonction gerant l'onglet TRANSFER */
	void OnDbleClickFileServer(wxTreeEvent& event);
	void OnDbleClickTransferServer(wxTreeEvent& event);
	void OnRightLeftSashChangeServer(wxNotebookEvent &event);
	void CreateItemServer();
	void DeleteItemServer();
	void ReloadExploServer();
	void DownloadFileServer();
	void LaunchDownload(wxListEvent& event);
	void LaunchUpload(wxListEvent& event);
	void CheckPreviewExtension(wxListEvent& event);
	void FillFolder(wxString rep); /*wxString folder, wxTreeItemId id);*/
	void UpLevelClient();
	void UpLevelServer();
	void RefreshServerPath();
	void GetFileInfo(wxListEvent& event); /// cette fontion va appeler les info du fichier

	void OnDbleClickFileClient(wxTreeEvent& event);
	void OnDbleClickTransferClient(wxTreeEvent& event);
	void OnTransferSashChange(wxNotebookEvent &event);
	void OnRightLeftSashChangeClient(wxNotebookEvent &event);
	void CreateItemClient();
	void DeleteItemClient();
	void ReloadExploClient();
	void UploadFileClient();

	void GetDraggedItemNameClient();
	void GetDraggedItemNameServer();

	void pause_file();
	void erase_file();
	void play_file(wxListEvent& event);
	void OnSelected(wxListEvent& event);

	/************************************/

private:
    wxLog *m_logTargetOld;
    DECLARE_EVENT_TABLE()
};
