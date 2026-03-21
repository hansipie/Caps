class CServData
{
public:
	CServData(void);
	~CServData(void);

	wxString				Serv_prompt;
	wxString				Serv_name;
	wxString				Serv_ip;
	bool					Serv_capsproto;

	int						CurentSelection;
	vector<CUser *>			Users;
	wxString				PublicChat;
	vector<wxString>		PrivChatTitle;
	vector<wxString>		PrivChat;
	vector<int>				PrivChatId;
	wxString				Server_version;

	int	 user_type;
	bool refuse_priv_chat;
	bool refuse_chat;
	bool away;

	bool deletefile;
	bool uploadfile;
	bool downloadfile;
	bool renamefile;
	bool movefile;
	bool createfolder;
	bool deletefolder;
	bool renamefolder;
	bool movefolder;
	bool readchat;
	bool sendchat;
	bool openchat;
	bool closechat;
	bool showinlist;
	bool createuser;
	bool deleteuser;
	bool openuser;
	bool modifyuser;
	bool changeownpassword;
	bool sendprivatemessage;
	bool newsreadarticle;
	bool newspostarticle;
	bool disconnectuser;
	bool cannotbedisconnect;
	bool getclientinfo;
	bool uploadanywhere;
	bool anyname;
	bool noagreement;
	bool getfilecomment;
	bool setfoldercomment;
	bool viewdropboxes;
	bool makealias;
	bool broadcast;
	bool newsdeletearticle;
	bool newscreatecategory;
	bool newsdeletecategory;
	bool newscreatefolder;
	bool newsdeletefolder;

};
