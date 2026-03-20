#ifndef __serv_hpp
#define __serv_hpp

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <list>
#include <map>
#include <vector>
#include <string>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <Magick++.h>

#ifdef __WIN32__
 #include <ws2tcpip.h>
 #pragma comment(lib, ws2_32.lib)
#else 
 #include <netinet/in.h>
 #include <netdb.h>
 #include <sys/socket.h>
 #include <sys/wait.h>
 #include <arpa/inet.h>
 #include <sys/resource.h>
 #include <pthread.h>
#endif

using namespace std;
//using namespace Magick;
 
#include "Systypes.hpp"
#include "Hotline.hpp"
#include "FlFileObj.hpp"
#include "Transaction.hpp"
#include "MyFile.hpp"
#include "CBuffer.hpp"
#include "CUser.hpp"
#include "CDwlUpl.hpp"

#ifdef __WIN32__
 #define SHUT_RDWR SD_BOTH 
#else
 #define INVALID_SOCKET -1
 #define SOCKET_ERROR -1
#endif

#define MYMAX(a,b) ( (a>b)?(a):(b) )

#define DEBUG_L0 true // server activity
#define DEBUG_L1 false // code debugging
#define DEBUG_L2 false // show transactions
#define DEBUG_L3 false // show buffers

//
// Class CServ
//

class CClServ;

class CServ
{
  // class attibutes
  
 public:
  char **env;
  int baseport;		// the port I'm listening regular transactions on
  int baseport1;	// the port I'm listening regular transactions on
  
  int              max_userId;
  vector<int>      fds_used;
  
  int BPsockfd;			// listen on sock_fd
  int BPsockfd1;		// listen on sock_fd

  fd_set readfds, writefds;	// I/O descriptors
  
  struct sockaddr_in	my_addr;	// my address information

  list<CUser>		AccountsLst;
  list<CBuffer>		BroadCastLst;
  list<CBufferTo>	MsgToLst;
  map<int,int>		RefTable;
  map<int,bool>		RefTableCaps;
  map<int,string>	RefTableFile;
  map<int,u_int32_t>	RefTableResume;
  map<string,string>	ExtToType;
  map<string,string>	TypeToCrea;
  map<int,CClServ*>	ClientsTable;		// clients table
  
  string		MsgBoard;

  struct
  {
    string	ServerName;
    int		Port;
    string	IPaddr;
    string	Agreement;
    string	FilesFolder;
    string	FilesType;
    string	Videolan;
  }		    ServConf;
  
private:
#ifdef __WIN32__
  WSADATA m_wsaData;
#endif
  
  // class methods
public:
  CServ();		// constructor
  ~CServ();		// destructor

  void		Session();
  void		KillClient(int);
  void		closesocket(int);	// close the socket
  void		LoopZ();

  void		set_non_blocking ( int, const bool);

  u_int16_t	ActionAgreed(CUser *);
  void		ActionBroadcast();
  void		ActionMsgTo();
  bool		ActionDisconnectUser(u_int16_t);
  list<CBuffer>	ActionGetUsrNameLst();
  list<CBuffer>	ActionGetAllAccounts();
  unsigned char	btoc(string);
  vector<char>  BitsField(string);
  void		AddUser(CUser);
  bool		DelUser(string);
  CUser		*GetUser(u_int16_t);
  CUser		*GetUser(string);
  bool		SetUser(CUser);
  bool	        UserRPM(u_int16_t);
  string	UserName(u_int16_t);
  string	GetUserInfo(CUser *);
  string	GetUserIP(u_int16_t);
  string	GetTypeStr(string);
  string	GetCreaStr(string);
  void		SetNewUserAccess(string);

private:
  void		alloc_CClient(int fd);
  void		Socket();
  void		Bind();
  void		Listen();
  bool		Connect(int, int, int, string);
  void		ClientsInitFDS();
  void		InitFilesTypes();
  void		InitUsers();
  void		InitServer();
  void		ParseConfLine(string);
  string	get_servername(string);
  int		get_serverport(string);
  string	get_path(string, int);
  int		GetAccounts(string);
  void		GetFilesType();
  void		ParseAccount(string);
  void		ParseAccountLine(string);
  string	get_stringval(string, string);
  int		get_intval(string, string);
  int		First_free_fd();
  int		Max_fd_used();
  string	get_HLCode(char*, int);
  void		GetLower(string&);
};

//
// Class CClServ
//

// realloc C++ :
// char buff
// buff = new (buff) char[X]

class CClServ
{
private:
  CServ		*Server;
  int		ServMesFlag;
  // -1: error
  // 0: instant message
  // 1..n: server message or admin message
  int		step;

  /*temp vars used to save globals values for fonctions*/
  CField	*c_tempfield;
  char		*c_tempbuff;
  string	c_tempstr;
  string	c_error;
  u_int16_t	c_tempint16;
  u_int32_t	c_tempint32;
  int		c_tempint;
  bool		c_tempbool;
  vector<int>	c_tempvectorint;
  
  int		m_ref;
  u_int32_t	m_resumept;
  
  typedef void		(CClServ::*MakeTransFunc)(int);  

  typedef bool		(CClServ::*DUFunc)();  

  list<CTransacLst>	RecvList;
  vector<MakeTransFunc> TabPtrFunc;
  vector<DUFunc>	TabPtrFuncDU;

 public:
  CUser		*User;
  bool		caps_cli;	//client is a caps client
  int		state;		//connection state
  int		PortType;	//connection port type
  string	IPAdress;      //IP adresse
  vector<int>	IdTransac;
  string	filefortransfert;
  bool		poison;
  CDwlUpl	*DwlUpl;

  struct sockaddr_in	dest_addr;         // used for datagram sending,
                                        // sure why we can't use their_addr, oh wel
  list<CBuffer>		SendList;
  
  CClServ(CServ*, int);
  ~CClServ();
  
  void	session(int);
  void	sessionBP(int);
  void	sessionBP1(int);
  void	Handshake();
  void	DUHandshake();
  bool	DwlRequest();
  bool	PrvRequest();
  bool	UplRequest();
  void  EndDlwUpl();
  void	set_send();
  int	GetId()
  {
    return (User->userId);
  };
  void	MakeSetNewAccess(int); 

 private:
  int	FreeId();
  int	WhatToSend();
  int	CliIsCaps(char *);
  int	analyse_rcvd(CTransac *);
  int	Handcorruption(char *);
  bool	DUHandcorruption(char *);
  int	readref(char*);
  int	readsize(char*);

  bool	DwlAction();
  bool	UplAction();
  bool	PrvAction();

  int	session_in();
  void	session_out();
  int	send_datagram(char *, int);        // send a datagram packet
  int	recv_datagram(char *, int);         // recv datagram
  void	set_recv();
  void  print_ascii(char*, int);
  void  print_hexa(char*, int);
  string	get_HLCode(char*, int);
  void	Suicide();
  void  Kill_fd();
  char	*read_agreement(char *);

  int	ReplyType(CTransac *, int);
  int	ShortReply(CTransac *, int);
  int	LongReply(CTransac *, int);
  int	NoReply(CTransac *, int);
  
  void	FinalizeBuffer(CTransactions);

  void	MakeShortReply(int);
  void	MakeLoginReply(int);
  void	MakeGetUsrNameLstReply(int);
  void  MakeAgreement(int);
  void	MakeUserAccess(int);
  void	MakeError(int);
  void	MakeUserChange(int);
  void  MakeUserDelete(int);
  void  MakePublicChat(int);
  void	MakeGetFileNameLstReply(int);
  void	MakeServerMessage(int);
  void	MakeDownloadFileReply(int);
  void	MakeUploadFileReply(int);
  void	MakeGetAllAccountsReply(int);
  void	MakeSaveAllAccountsReply(int);
  void	MakeGetMessageReply(int);
  void	MakeNewMessage(int);
  bool	MakeDisconnectMsg(int);
  bool	MakeGetUserReply(int);
  bool	MakeGetFileInfoReply(int);
  bool	MakeGetCliInfoTxtReply(int);
  bool	MakeBroadCast(int);

  void	ReturnRightsError(int, int);
  
  bool	ActionLogin(CTransac *);
  bool	ActionSetClUsrInf(CTransac *);
  bool	ActionAgreed(CTransac *);
  void	ActionUserInfos(CTransac *);
  void	ActionBroadcast(CTransac *);
  void  ActionPublicChat(CTransac *);
  void	ActionGetFileNameLst(CTransac *);
  bool	ActionInstantMessage(CTransac *);
  bool	ActionDownloadFile(CTransac *);
  void	ActionUploadFile(CTransac *);
  bool	ActionDelFile(CTransac *);
  bool	ActionNewFolder(CTransac *);
  bool	ActionMoveFile(CTransac *);
  void	ActionGetFileInfo(CTransac *);
  bool	ActionSetFileInfo(CTransac *);
  bool  ActionDisconnectUser(CTransac *);
  bool  ActionGetCliInfoTxt(CTransac *);
  void	ActionGetAllAccounts(CTransac *);
  void	ActionSaveAllAccounts(CTransac *);
  void	ActionOldPostNews(CTransac *);
  void	ActionNewUser(CTransac *);
  bool	ActionDelUser(CTransac *);
  void	ActionGetUser(CTransac *);
  bool	ActionSetUser(CTransac *);

  /* Tools */
  vector<char>		BitsField(string);
  string		ctob(unsigned char);
  unsigned char		btoc(string);
  void			ChangeUserFlags(int, string);
  void			SetFlags113(string);
  void			ls(string);
  int			FileSize(string);
  void			UpdateUserFlags(bool);
  string		GetExtension(string);
  void			ClearTemps();
  bool			Is_movie(string);

  /* Cplx Fields */
  vector<string>	Field202(char *, int);
  u_int32_t		Field203(char *, int);

  int	get_FFO(char *, int);

};

#endif
