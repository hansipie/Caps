#include "Serv.hpp"

void CClServ::FinalizeBuffer(CTransactions tosend)
{
  if (DEBUG_L2)
    tosend.show_transac();
  tosend.build_buff();
  SendList.push_back(CBuffer(tosend.buff, tosend.buffsize));
}

//
// transactions reponses du serveur
//


void CClServ::MakeError(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: error from server\n";
  CTransactions tosend(Id);
  //tosend.build_hdr(HL_ERROR, HDR_RPLY);
  tosend.build_hdr(NO_TYPE, HDR_RPLY);
  tosend.build_field_s(ID_ERROR, (char *)c_error.c_str());
  FinalizeBuffer(tosend);
  c_error.clear();
}

void CClServ::ReturnRightsError(int Id, int type)
{
  if (DEBUG_L0)
    cout << "Rights error for transaction " << type << endl;
  c_error = "error: not enougth rights to performe this action.";
  MakeError(Id);
}


void CClServ::MakeShortReply(int Id)
{
  if (DEBUG_L0)
    cout << "ShortReply\n";
  CTransactions tosend(Id);
  tosend.build_hdr(NO_TYPE, HDR_RPLY);
  FinalizeBuffer(tosend);
}

void CClServ::MakeGetMessageReply(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: Get Message reply from server\n";
  CTransactions tosend(Id);
  tosend.build_hdr(NO_TYPE, HDR_RPLY);
  tosend.build_field_s(ID_DATA, (char *)Server->MsgBoard.c_str());
  FinalizeBuffer(tosend);  
}

void CClServ::MakeLoginReply(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: login reply from server\n";
  CTransactions tosend(Id);
  tosend.build_hdr(NO_TYPE, HDR_RPLY);
  tosend.build_field_i(ID_VER, VERSION);
  tosend.build_field_i(ID_BANNER_ID, 0);
  tosend.build_field_s(ID_SERV_NAME, (char *)Server->ServConf.ServerName.c_str());
  FinalizeBuffer(tosend);
}

void CClServ::MakeGetUsrNameLstReply(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: user list reply from server\n";
  CTransactions tosend(Id);
  tosend.build_hdr(NO_TYPE, HDR_RPLY);
  /*liste des utilisateurs*/
  list<CBuffer> unamelst = Server->ActionGetUsrNameLst();

  for (list<CBuffer>::iterator it= unamelst.begin();
       it != unamelst.end(); it++)
    {
      tosend.build_field_b(ID_USR_NAME_INFO, it->getbuff(), it->size);
      it->delbuff();
    }
  FinalizeBuffer(tosend); 
}

void CClServ::MakeGetFileNameLstReply(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: Get File Name List reply from server" << endl;

  DIR		*dirp;
  struct dirent *dirent;
  struct stat	sb;
  string	type, filepath, tmpfilepath, crea;
  char		*fieldata, *tmpdata;
  int		size;
  u_int32_t	tmp_long;

  CTransactions tosend(Id);
  tosend.build_hdr(NO_TYPE, HDR_RPLY);

  filepath = c_tempstr;
  c_tempstr.clear();
  if (DEBUG_L0)
    cout << "Path: " << filepath << endl;
  if ((dirp = opendir(filepath.c_str())) == NULL)
    {
      perror("opendir() error");
      c_error.assign("Error openning directory, not found.");
      MakeError(Id);
      return;
    }
  else
    {
      while ((dirent = readdir(dirp)))
	{
	  if (DEBUG_L1)
	    cout << "dirent->d_name control [" << dirent->d_name << "]" << endl;
	  if (!strcmp(dirent->d_name,".") || !strcmp(dirent->d_name,".."))
	    continue;
	  tmpfilepath = filepath;
	  tmpfilepath.append(dirent->d_name);
	  if (DEBUG_L1)
	    cout << "stat for " << tmpfilepath << endl;
	  if (stat(tmpfilepath.c_str(), &sb) == -1)
	    perror("stat() error");
	  else
	    {
	      size = strlen(dirent->d_name);
	      fieldata = new char[20 + size];
	      tmpdata = fieldata;
	      if (sb.st_mode & S_IFDIR)
		{
		  type = "fldr";
		  if (DEBUG_L1)
		    cout << dirent->d_name << "\t" << sb.st_size << "\t" << type <<endl;
		  // type
		  memcpy(tmpdata, type.c_str(), 4);
		  tmpdata += 4;
		  // creator
		  memset(tmpdata, 0, 4);
		  tmpdata += 4;
		}
	      else
		{		  
		  type = Server->GetTypeStr(dirent->d_name);
		  crea = Server->GetCreaStr(dirent->d_name);
		  if (DEBUG_L1)
		    cout << dirent->d_name << "\t" << sb.st_size << "\t" << type  << "\t" << crea <<endl;
		  // type
		  memset(tmpdata, 0, 4);
		  memcpy(tmpdata, type.c_str(), strlen(type.c_str()));
		  tmpdata += 4; 
		  // creator
		  memset(tmpdata, 0, 4);
		  memcpy(tmpdata, crea.c_str(), strlen(crea.c_str()));
		  tmpdata += 4; 
		}
	      // file size
	      tmp_long = htonl(sb.st_size);
	      memcpy(tmpdata, &tmp_long, sizeof(u_int32_t));
	      tmpdata += sizeof(u_int32_t);
	      // ??
	      memset(tmpdata, 0, sizeof(u_int32_t));
	      tmpdata += sizeof(u_int32_t); 
	      // name script
	      memset(tmpdata, 0, sizeof(u_int16_t));
	      tmpdata += sizeof(u_int16_t); 
	      // name size
	      tmp_long = htons(size);
	      memcpy(tmpdata, &tmp_long, sizeof(u_int16_t));
	      tmpdata += sizeof(u_int16_t);
	      // file name
	      memcpy(tmpdata, dirent->d_name, size);
	      
	      tosend.build_field_b(ID_FILE_NAME_INFO, fieldata, 20 + size);      
	      
	      type.clear();
	      delete [] fieldata;
	    }
	}
    }
  FinalizeBuffer(tosend); 
}

void CClServ::MakeDownloadFileReply(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: Download File reply from server" << endl;
  filefortransfert.assign(c_tempstr);
  c_tempstr.clear();
  bool localbool = c_tempbool;
  c_tempbool = false;
  if (DEBUG_L0)
    cout << " filename: " << filefortransfert << endl;
  if (DEBUG_L1)
    {
      cout << " filename size: " << filefortransfert.length() << endl;
      cout << " resume point: " << c_tempint32 << endl;
    }
  int filesize = FileSize(filefortransfert);
  time_t seconds;

  time(&seconds);
  srand((u_int32_t) seconds);
  
  CTransactions tosend(Id);
  tosend.build_hdr(NO_TYPE, HDR_RPLY);
  tosend.build_field_i(ID_TRANS_SIZE, 
		       FFO_SIZE + filefortransfert.length() + filesize);
  tosend.build_field_i(ID_FILE_SIZE, filesize);
  int ref = rand();
  if (DEBUG_L1)
    cout << " transfer ref: " << ref << endl;
  //faire une fonction dans CServ qui fait ca:
  if (localbool)
    Server->RefTable[ref] = PRV;
  else
    Server->RefTable[ref] = DWL;
  Server->RefTableCaps[ref] = caps_cli;
  Server->RefTableFile[ref] = filefortransfert;
  Server->RefTableResume[ref] = c_tempint32;
  c_tempint32 = 0;
  // end
  tosend.build_field_i(ID_REF_NBR, ref);	// define refence
  tosend.build_field_i(ID_WAIT_COUT, 0);	// define waiting count
  FinalizeBuffer(tosend);
}

void CClServ::MakeUploadFileReply(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: Upload File reply from server" << endl;
  filefortransfert.assign(c_tempstr);
  if (DEBUG_L0)
    cout << " filename: " << filefortransfert << endl;

  time_t seconds;

  time(&seconds);
  srand((u_int32_t) seconds);
  
  CTransactions tosend(Id);
  tosend.build_hdr(NO_TYPE, HDR_RPLY);
  int ref = rand();
  if (DEBUG_L1)
    cout << " Setting RefTables: " << ref << endl;
  //faire une fonction dans CServ qui fait ca:
  Server->RefTable[ref] = UPL;
  Server->RefTableCaps[ref] = caps_cli;
  Server->RefTableFile[ref] = filefortransfert;
  Server->RefTableResume[ref] = 0;
  // end
  tosend.build_field_i(ID_REF_NBR, ref);	// define refence
  FinalizeBuffer(tosend);
}

bool CClServ::MakeGetFileInfoReply(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: Get File Info reply from server\n";
   
  string file(c_tempstr);
  c_tempstr.clear();
  c_tempint = 0;

  int starti = file.find_last_of("/") + 1;
  int endi = file.length();
  string filename = file.substr(starti, endi - starti);
  if (DEBUG_L0)
    cout << " filename: " << filename << " (length: " << filename.length() << ")" << endl;
  if (DEBUG_L1)
    cout << " stat for " << file << endl;
  struct stat	sb;
  if (stat(file.c_str(), &sb) == -1)
    {
      perror(" stat() error");
      return false;
    }
  else
    {
      CTransactions tosend(Id);
      tosend.build_hdr(NO_TYPE, HDR_RPLY);
      if (sb.st_mode & S_IFDIR)
	{
	  tosend.build_field_s(ID_FILE_TYPE_STR, (char*)"Folder");
	  tosend.build_field_s(ID_FILE_CREA_STR, (char*)"n/a");
	  tosend.build_field_b(ID_FILE_TYPE, (char*)"fldr", 4); // a verifier
	}
      else
	{
	  tosend.build_field_b(ID_FILE_TYPE, (char*)"othr", 4); // a verifier
	  string type = Server->GetTypeStr(filename);
	  tosend.build_field_s(ID_FILE_TYPE_STR, (char *)type.c_str());
	  string crea = Server->GetCreaStr(filename);
	  tosend.build_field_s(ID_FILE_CREA_STR, (char *)crea.c_str());  
	  tosend.build_field_i(ID_FILE_SIZE, sb.st_size);
  	}
      tosend.build_field_s(ID_FILE_NAME, (char *)filename.c_str());
      tosend.build_field_b(ID_FILE_CREA_DAT, (char*)"", 0);
      tosend.build_field_b(ID_FILE_MOD_DAT, (char*)"", 0);
      tosend.build_field_s(ID_FILE_COMM, (char *)Server->ServConf.ServerName.c_str());
      FinalizeBuffer(tosend);
      return true;
    }
}

bool CClServ::MakeGetCliInfoTxtReply(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: Get Client Info Text Reply" << endl;
  CUser *user = Server->GetUser(c_tempint16);
  c_tempint16 = 0;
  if (user == NULL)
    return false;
  string data = Server->GetUserInfo(user);
  CTransactions tosend(Id);
  tosend.build_hdr(NO_TYPE, HDR_RPLY);
  tosend.build_field_s(ID_USR_NAME, (char*)user->name.c_str());
  tosend.build_field_s(ID_DATA, (char *)data.c_str());
  FinalizeBuffer(tosend);
  return true;
}

bool CClServ::MakeGetUserReply(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: Get User Reply" << endl;
  CUser *User = Server->GetUser(c_tempstr);
  if (User == NULL)
    return false;
  CTransactions tosend(Id);
  tosend.build_hdr(NO_TYPE, HDR_RPLY);
  string login = get_HLCode((char *)User->login.c_str(), User->login.length());
  tosend.build_field_s(ID_USR_LOG, (char *)login.c_str());
  string password = get_HLCode((char *)User->password.c_str(), User->password.length());
  tosend.build_field_s(ID_USR_PASS, (char*)password.c_str());
  
  tosend.build_field_s(ID_USR_NAME, (char*)User->confname.c_str());
  string bits; 
  bits.assign(64, '0');
  for (int i = 0; i < 41; i++)
    bits.replace(i, 1, (User->access[i]?"1":"0"));
  vector<char> bitsfield = BitsField(bits);  
  tosend.build_field_b(ID_USR_ACCESS, bitsfield, 8);
  FinalizeBuffer(tosend);
  return true;
}

//
// transactions initiees par le serveur
//

void CClServ::MakeNewMessage(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: New Message from server\n";
  CTransactions tosend(Id);
  tosend.build_hdr(NEW_MSG, HDR_RQST);
  tosend.build_field_s(ID_DATA, (char *)Server->MsgBoard.c_str());
  if (DEBUG_L2)
    tosend.show_transac();
  tosend.build_buff();
  if (DEBUG_L1)
    cout << "Pushing new broadcast in server's broadcast list." << endl;
  Server->BroadCastLst.push_back(CBuffer(tosend.buff, tosend.buffsize));  
}

void CClServ::MakeAgreement(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: show agreement from server" << endl;
  CTransactions tosend(Id);
  tosend.build_hdr(SHOW_AGREEM, HDR_RQST);
  char *agreement = read_agreement((char *)Server->ServConf.Agreement.c_str()); 
  tosend.build_field_s(ID_DATA, agreement);
  FinalizeBuffer(tosend); 
}

void  CClServ::MakeUserAccess(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: user access from server" << endl;

  string bits; 
  bits.assign(64, '0');
  for (list<CUser>::iterator it = Server->AccountsLst.begin();
       it !=  Server->AccountsLst.end();
       it++)
    {
      if ((it->login == User->login) && (it->password == User->password))
	{
	  for (int j = 0; j < 41; j++)
	    User->access[j] = it->access[j];
	  break;  
	}
    }
  UpdateUserFlags(true);
  for (int i = 0; i < 41; i++)
    bits.replace(i, 1, (User->access[i]?"1":"0"));
  vector<char> bitsfield = BitsField(bits);  
  CTransactions tosend(Id);
  tosend.build_hdr(USER_ACCES, HDR_RQST);
  tosend.build_field_b(ID_USR_ACCESS, bitsfield, 8);
  FinalizeBuffer(tosend);
} 

void	CClServ::MakeSetNewAccess(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: new user access from server" << endl;
 
  Id = FreeId();
  string bits; 
  bits.assign(64, '0');
  for (list<CUser>::iterator it = Server->AccountsLst.begin();
       it !=  Server->AccountsLst.end();
       it++)
    {
      if ((it->login == User->login) && (it->password == User->password))
	{
	  for (int j = 0; j < 41; j++)
	    User->access[j] = it->access[j];
	  break;  
	}
    }
  UpdateUserFlags(false);
  for (int i = 0; i < 41; i++)
    bits.replace(i, 1, (User->access[i]?"1":"0"));
  vector<char> bitsfield = BitsField(bits);  
  CTransactions tosend(Id);
  tosend.build_hdr(USER_ACCES, HDR_RQST);
  tosend.build_field_b(ID_USR_ACCESS, bitsfield, 8);
  FinalizeBuffer(tosend);
  MakeUserChange(FreeId());
  Server->ActionBroadcast();
}

void CClServ::MakeUserChange(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: notify user change" << endl;

  CTransactions tosend(Id);
  tosend.build_hdr(NOT_CHANGE_USR, HDR_RQST);
  tosend.build_field_i(ID_UID, User->userId);
  tosend.build_field_i(ID_USR_ICON, User->iconId);
  tosend.build_field_i(ID_USER_FLAG, User->userFlags);
  tosend.build_field_s(ID_USR_NAME, (char*)User->name.c_str());
  if (DEBUG_L2)
    tosend.show_transac();
  tosend.build_buff();
  if (DEBUG_L1)
    cout << "Pushing new broadcast in server's broadcast list." << endl;
  Server->BroadCastLst.push_back(CBuffer(tosend.buff, tosend.buffsize));  
}

void CClServ::MakeUserDelete(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: notify user delete" << endl;

  CTransactions tosend(Id);
  tosend.build_hdr(NOT_DEL_USR, HDR_RQST);
  tosend.build_field_i(ID_UID, User->userId);
  if (DEBUG_L2)
    tosend.show_transac();
  tosend.build_buff();
  if (DEBUG_L1)
    cout << "Pushing new broadcast in server's broadcast list." << endl;
  Server->BroadCastLst.push_back(CBuffer(tosend.buff, tosend.buffsize));  
}

bool CClServ::MakeBroadCast(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: make broadcast" << endl;

  if (c_tempstr.empty())
    {
      cerr << "error: Nothing to broadcast." << endl;
      return false;
    }
  CTransactions tosend(Id);
  tosend.build_hdr(USER_BROADCAST, HDR_RQST);
  tosend.build_field_i(ID_UID, User->userId);
  tosend.build_field_s(ID_USR_NAME, (char *)User->name.c_str());
  tosend.build_field_s(ID_DATA, (char *)c_tempstr.c_str());
  c_tempstr.clear();
  if (DEBUG_L2)
    tosend.show_transac();
  tosend.build_buff();
  if (DEBUG_L1)  
    cout << "Pushing new broadcast in server's broadcast list." << endl;
  Server->BroadCastLst.push_back(CBuffer(tosend.buff, tosend.buffsize));
  return true;
}

void CClServ::MakePublicChat(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: make public chat" << endl;

  if (c_tempstr.empty())
    { 
      cerr << "error: Nothing to broadcast." << endl;
      return; 
    }
  string chat;
  char c = 13;
  chat.assign(1, c);
  chat.append((User->name + ": ") + c_tempstr);
  c_tempstr.clear();
  CTransactions tosend(Id);
  tosend.build_hdr(CHAT_MSG, HDR_RQST);
  tosend.build_field_b(ID_DATA, (char *)chat.c_str(), chat.length());
  if (DEBUG_L2)
    tosend.show_transac();
  tosend.build_buff();
  if (DEBUG_L1)  
    cout << "Pushing new public chat in server's broadcast list." << endl;
  Server->BroadCastLst.push_back(CBuffer(tosend.buff, tosend.buffsize));  
}

void CClServ::MakeServerMessage(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: make server message" << endl;

  CTransactions tosend(Id);
  tosend.build_hdr(SERV_MSG, HDR_RQST);
  if (!Server->UserRPM(c_tempint16))
    {
      switch (ServMesFlag)
	{
	case -1:
	  cout << "error: Flag Server message type not set" << endl;
	  break;
	case 0:
	  tosend.build_field_i(ID_UID, User->userId);
	  tosend.build_field_s(ID_USR_NAME, (char *)User->name.c_str());
	  tosend.build_field_i(ID_OPT, 1); /* valeure a generer ... c pas encore ca */
	  tosend.build_field_s(ID_DATA, (char *)c_tempstr.c_str());
	  //tosend.build_field_b(ID_QUOT_MSG, ,);
	  break;
	default:      
	  tosend.build_field_s(ID_DATA, (char *)c_tempstr.c_str());
	  tosend.build_field_i(ID_CHAT_OPT, ServMesFlag);
	}
    }
  else
    {
      ServMesFlag = 1;
      c_tempstr.assign(Server->UserName(c_tempint16) + RPM);
      tosend.build_field_s(ID_DATA, (char *)c_tempstr.c_str());
      tosend.build_field_i(ID_CHAT_OPT, ServMesFlag);
      c_tempint16 = User->userId;
    }
  if (DEBUG_L2)
    tosend.show_transac();
  tosend.build_buff();
  if (DEBUG_L1) 
    cout << "Pushing new message in server's message list." << endl;
  Server->MsgToLst.push_back(CBufferTo(c_tempint16, tosend.buff, tosend.buffsize));  
  c_tempint16 = 0;
  c_tempstr.clear();
  ServMesFlag = -1;
}

void	CClServ::MakeGetAllAccountsReply(int Id)
{
  if (DEBUG_L0)
    cout << "transaction step: get all accounts reply\n";
  CTransactions tosend(Id);
  tosend.build_hdr(NO_TYPE, HDR_RPLY);
  /*liste des utilisateurs*/
  list<CBuffer> alluser = Server->ActionGetAllAccounts();
  for (list<CBuffer>::iterator it= alluser.begin();
       it != alluser.end(); it++)
    {
      tosend.build_field_b(ID_DATA, it->getbuff(), it->size);
      it->delbuff();
    }
  FinalizeBuffer(tosend); 
  
}

void	CClServ::MakeSaveAllAccountsReply(int Id)
{ 
  if (DEBUG_L0)
    cout << "transaction step: save all users reply\n";
}

//
// transactions pas encore utilisees
//

bool CClServ::MakeDisconnectMsg(int Id)
{
  if (c_tempint16 == 0)
    {
      cerr << "error: no destination Id." << endl;
      return false;
    }
  CTransactions tosend(Id);
  tosend.build_hdr(DISC_MSG, HDR_RQST);
  tosend.build_field_b(ID_DATA, (char *)c_tempstr.c_str(), c_tempstr.length());
  if (DEBUG_L2)
    tosend.show_transac();
  tosend.build_buff();
  if (DEBUG_L1)  
    cout << "Pushing new message in server's message list." << endl;
  Server->MsgToLst.push_back(CBufferTo(c_tempint16, tosend.buff, tosend.buffsize));  
  c_tempint16 = 0;
  c_tempstr.clear();
  return true;
}

