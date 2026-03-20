// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

enum
{
    CONTROLS_QUIT   = 100,
    CONTROLS_TEXT,
    CONTROLS_ABOUT,
    CONTROLS_BROADCAST,
    CONTROLS_CLEAR_LOG,
    CONTROLS_CONNECT,
    CONTROLS_CONNECT_CLIENT,
    CONTROLS_DISCONNECT,
    CONTROLS_HELP,
    CONTROLS_OPTIONS,
	CONTROLS_ADMIN,
    SOCKET_ID,
    LIST_SERVER
};

class MyPanel;
class CRep;
class CFileList;
class CAgree;
class CAdmin;

class MyFrame: public wxFrame
{
public:
	//notre nom de scene et notre icone
	wxString Name;
	wxString Icone;
	//ca sert au client console uniquement
	wxString Client_Host;
	wxString Client_Port;
	//dernieres variables enregistrees par la fenettre de connection au serveur 
	wxString			Serv_User; 
	wxString			Serv_Passwd;
	wxString			Serv_Host;
	wxString			Serv_Port;
	bool				connect_opt_ag;
	//wxComboBox			*list_serv;
	wxListBox			*list_bookmark;
	MyPanel				*m_panel;
	wxStaticBitmap		*bitmap_title;
	wxStaticBitmap		*degrade_title;
	CAdmin				*m_admin;

	wxString 			buf;
	wxString 			line_cmd;
	vector<wxString>	tab_line_cmd;
	vector<wxString>	tab_arg_line_cmd;
	vector<wxString>	longcmd;

	// Variables d'implementation des explorateurs de fichier Client / serveur
	wxString			CurrentRep;
	wxTreeItemId		CurrentId;
	vector<wxString>	tab_line_cmd_last; // sauvegarde de tab_line_cmd pour remplir les repertoires (engine.cpp)
	int					fill_in_progress; // ce flag indik kon remplit actuellement un sous repertoire (1 ou 0)
	CRep				*CurrentServRepRoot;
	CRep 				*CurrentServRepList;
	CFileList			*CurrentServFileList;
	void				FillSousRep(wxString CurrentRep, wxTreeItemId CurrentId);
	//void				FillFolder(wxString rep); /*wxString folder, wxTreeItemId id);*/
//	wxString			*ServerPath;

	wxToolBar			*toolbar;
	vector<wxString>	account_list;

	///////////////////////// variables de previews
	unsigned long		preview_size;
	unsigned long		preview_curr_size;
	wxFile				preview_file;
	char				*tmpbuf;

	CRep				*ServRepRoot;
	CRep 				*ServRepList;
	CFileList			*ServFileList;

	CRep				*ClientRepRoot;
	CRep 				*ClientRepList;
	CFileList			*ClientFileList;
	CAgree				*Agree;
	/* La string de statut en bas a gauche de notre fenetre*/
	//wxString			StatusText;
	wxString			Broadcast_String;
	int					BroadcastFlag;

	/*  VARIABLE DU MENU */
	wxMenu *file_menu;
	wxMenu	*connect_menu;
	wxMenu *help_menu;

	bool			mess_join_leave_chat;
	bool			accept_priv_mess;
	bool			accept_priv_chat;
	bool			automatic_response;
	bool			log_mess;
	bool			auto_agree;

	/*VARIABLE CRYPTO */

	bool			opt_crypto;
	wxString		Pass_crypto;

	// liste chainee de commandes et flag
	vector<wxString>	outputcmd;
	bool				busy;

	int					is_connected_console;
	bool				login_in_progress;
	wxString			connexion_name;
	bool				connexion_capsproto;
	wxString			prompt;

    MyFrame(const wxChar *title, int x, int y);
    ~MyFrame();
 
	void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnSendBroadcast(wxCommandEvent& event);
	void OnOptions(wxCommandEvent &event);
	void OnAdmin(wxCommandEvent &event);
	void OnConnect(wxCommandEvent& event);
	void OnConnect_client(wxCommandEvent& event);
    void OnClearLog(wxCommandEvent& event);
	void OnSocketEvent(wxSocketEvent& event);
	void CreateMyMenuBar();
	void CreateMyToolBar();
	void OnIdle( wxIdleEvent& event );
    void OnSize( wxSizeEvent& event );
    void OnMove( wxMoveEvent& event );
	void OnNewFrame (wxCommandEvent & event);
	void input_message(wxSocketEvent& event);
	void output_message(wxString msg);
	void auto_agreement();

	void relogin();
	void disconnecting(vector<wxString>::iterator i);
	void server_version(vector<wxString>::iterator i);
	void server_known(vector<wxString>::iterator i);
	void disconnected(vector<wxString>::iterator i);
	void server_list(vector<wxString>::iterator i);

	void agree();
	void connect_server(vector<wxString>::iterator i);
	void lost_console();
	void Affstruct();
	void receive_chat(vector<wxString>::iterator i);
	void OnDisconnect(wxCommandEvent& event);
	void user_set_list(vector<wxString>::iterator i);
	void file_client_set_list(vector<wxString>::iterator i);
	void file_set_list(vector<wxString>::iterator i);
	void user_access(vector<wxString>::iterator i);
	void get_login_list(vector<wxString>::iterator i);

	void user_change(vector<wxString>::iterator i);
	void change_serv();
	void receive_priv_chat(vector<wxString>::iterator i);
	//void auto_launch();
	void launch_server_box();
	void broadcast_msg(vector<wxString>::iterator i);
	void AffFileInfo(vector<wxString>::iterator i);  // Ouvre une box et afiche le file info

	void receive_server_msg(vector<wxString>::iterator i);
	void task_list(vector<wxString>::iterator i);

	void user_delete(wxString IdToDelete);

	// declaration fonctions CRYPTO
	
	char * tocharetoile(wxString str);
	char * crypto(wxString chaine, wxString cle, unsigned char cmd);
	unsigned char rcon(int i);
	unsigned char sub_s_box(unsigned char c, unsigned char cmd);
	unsigned char multi(char a, char b);

	int expand_key(unsigned char key[4*4], unsigned char w[4*4*11]);
	int select_key(unsigned char expanded_key[4*4*11], unsigned char round_key[4][4], int round);
	int byte_sub(unsigned char state[4][4], unsigned char cmd);
	int shift_row(unsigned char state[4][4], unsigned char cmd);
	int mix_column(unsigned char state[4][4], unsigned char cmd);
	int add_round_key(unsigned char state[4][4], unsigned char round_key[4][4]);
	int round(unsigned char state[4][4], unsigned char round_key[4][4], int round, unsigned char cmd);
	

	/** Declarratiobn des fcts de preview ***/
//	void	receive_preview(wxChar *stream, int bit_left);
	void IntToBox(int i);
	void preview_in_file(char *stream, int bit_left);
	/****************************************/

	
	void error_msg(vector<wxString>::iterator i);
	
	void get_user(vector<wxString>::iterator i);


	MyPanel *GetPanel() const { return m_panel; }
	wxSocketClient *m_sock;

	vector<wxString> StringToTab(wxString chaine,wxString sep);


private:
	bool     m_busy;
    DECLARE_EVENT_TABLE()
};
