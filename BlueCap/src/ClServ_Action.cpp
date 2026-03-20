#include "Serv.hpp"

bool	CClServ::ActionLogin(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Action Login\n";
  char *data;
  int flag = 0;
  User->login = "";
  User->password = "";
  User->version = 0;
  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      data = it->getdata();
      switch (it->id)
	{
	case ID_USR_ICON: /* if this field */
	  memcpy(&User->iconId, data, it->size);
	  User->iconId = ntohs(User->iconId);
	  flag++;
	  break;
	case ID_USR_LOG:
	  User->login = get_HLCode(data, (int)it->size);
	  if (DEBUG_L1)
	    cout << "User->login: " << User->login << endl;
	  break;
	case ID_USR_PASS:
	  User->password = get_HLCode(data, (int)it->size);
	  if (DEBUG_L1)
	    cout << "User->password: " << User->password << endl;
	  break;
	case ID_VER:
	  memcpy(&User->version, data, it->size);
	  User->version = ntohs(User->version);
	  break;
	case ID_USR_NAME: /* and this field */
	  User->name.assign(data, it->size);
	  flag++;
	  break;
	default:
	  std::cerr << "error: " << it->id << " is an unknown transaction field for Login\n";
	}
      delete [] data;
    }
  for (list<CUser>::iterator it2 = Server->AccountsLst.begin();
       it2 != Server->AccountsLst.end();
       it2++)
    {
      if ((it2->login == User->login) && (it2->password == User->password))
	{
	  /* then  agreed action */
	  if (flag == 2)
	    {
	      User->userId = Server->ActionAgreed(User);
	      User->connected = true;
	      if (DEBUG_L0)
		cout << " agreed... connection of user " << User->userId << endl;
	    }	    
	  return true;
	}
    }
  cerr << "error: " << User->login << " is an unknown login or password is incorrect [" << User->password << "]" <<endl;
  c_error = "error: login or password is incorrect.";
  MakeError(transac->id);
  set_send();
  poison = true;
  if (DEBUG_L1)
    cout << "user is poisoned" << endl;
  return false;
}

void	CClServ::ActionOldPostNews(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Old Post News\n";

  char			*data;

  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_DATA:
	  data = it->getdata();
	  Server->MsgBoard.append("\n------------------------------------\n");
	  Server->MsgBoard.append(data);
	  delete [] data;
	  break;
	default:
	  std::cerr << "error: " << it->id << " is an unknown transaction field for Delete File\n";
	}
    }
}

bool	CClServ::ActionSetClUsrInf(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Set Client User Infos\n";

  ActionUserInfos(transac);

  //update des user ds serv a faire
  return true;
}

bool	CClServ::ActionAgreed(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Action Agreed\n";
  
  ActionUserInfos(transac);
  
  User->userId = Server->ActionAgreed(User);
  User->connected = true;
  if (DEBUG_L0)
    cout << " agreed... connection of user " << User->userId << endl;
  return true;
}

void	CClServ::ActionUserInfos(CTransac *transac)
{
  char *data;
  string flags, tmp;
  u_int16_t opt;
  
  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      data = it->getdata();
      switch (it->id)
	{
	case ID_USR_NAME:
	  User->name.assign(data, it->size);
	  if (DEBUG_L1)
	    cout << " user name: " << User->name << endl;
	  break;
	case ID_USR_ICON:
	  memcpy(&User->iconId, data, it->size);
	  User->iconId = ntohs(User->iconId);
	  if (DEBUG_L1)
	    cout << " user iconId: " << User->iconId << endl;;
	  break;
	case ID_OPT:
	  memcpy(&opt, data, it->size);
	  opt = ntohs(opt);
	  // on suppose que les options sont codees sur 1 octet
	  // alors que en realite elles sont codees sur 2 octets, 
	  // car le premier octet est a 0
	  flags = ctob(opt);
	  SetFlags113(flags);
	  if (DEBUG_L1)
	    {
	      cout << " opt: " << opt << " - " << flags <<endl;
	      tmp =  (User->ar)?"true":"false";
	      cout << "  auto response: " << tmp <<endl; 
	      tmp = (User->rpc)?"true":"false";
	      cout << "  refuse private chat: " << tmp <<endl; 
	      tmp = (User->rpm)?"true":"false"; 
	      cout << "  refuse private message: " << tmp <<endl;
	    } 
	  break;
	case ID_AUTO_RESP:
	  User->auto_rep.assign(data, it->size);
	  if (DEBUG_L1)
	    cout << " auto response string: " << User->auto_rep << endl;;
	  break;
	default:
	  cerr << "error: " << it->id << " is an unknown transaction field\n";
	}
      delete [] data;
    }
}

void	CClServ::ActionBroadcast(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Action User broadcast\n";

  char		*data;
  string	message, name("");
  u_int16_t	uid = 0;
  
  //c_tempfield = &(transac->field.front()); 
  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_UID:
	  data = it->getdata();
	  memcpy(&uid, data, it->size);
	  it->deldata();
	  uid = ntohs(uid);
	  break;
	case ID_USR_NAME:
	  data = it->getdata();
	  name.assign(data, it->size);
	  it->deldata();
	  break;
	case ID_DATA:
	  data = it->getdata();
	  message.assign(data, it->size);
	  it->deldata();
	  break;
	default: 
	  std::cerr << "error: " << it->id << " is an unknown transaction field for Public Chat\n";
	}
    }
  if (DEBUG_L1)
    {
      cout << " name: " << name << endl;
      cout << " uid: " << uid << endl;
      cout << " message: " << message << endl; 
    }
  c_tempstr.assign(message);
}

void	CClServ::ActionPublicChat(CTransac *transac)
{
  char *data;

  if (DEBUG_L0)
    cout << "Action step: Action Public Chat\n";
  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_CHAT_OPT:
	  break;
	case ID_CHAT_ID:
	  break;
	case ID_DATA:
	  data = it->getdata();
	  c_tempstr.assign(data, it->size);
	  it->deldata();	  
	  break;
	default:
	  std::cerr << "error: " << it->id << " is an unknown transaction field for Public Chat\n";
	}
    }
}

bool CClServ::ActionDelFile(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Delete File\n";
  
  string		filename, pathstr;
  vector<string>	filepath;
  char			*data;

  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_FILE_NAME:
	  data = it->getdata();
	  filename.assign(data, it->size);
	  delete [] data;
	  break;
	case ID_FILE_PATH:
	  data = it->getdata();
	  filepath = Field202(data, it->size);
	  delete [] data;
	  break;
	default:
	  std::cerr << "error: " << it->id << " is an unknown transaction field for Delete File\n";
	}
    }
  pathstr = Server->ServConf.FilesFolder;
  for (vector<string>::iterator it = filepath.begin(); it != filepath.end(); it++)
    {
      pathstr.append(it->c_str());
      pathstr.append("/");
    }
  CMyFile Object(pathstr+filename);
  return (Object.Remove());
}

bool CClServ::ActionNewFolder(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: New Folder\n";

  string		filename, pathstr;
  vector<string>	filepath;
  char			*data;

  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_FILE_NAME:
	  data = it->getdata();
	  filename.assign(data, it->size);
	  delete [] data;
	  break;
	case ID_FILE_PATH:
	  data = it->getdata();
	  filepath = Field202(data, it->size);
	  delete [] data;
	  break;
	default:
	  std::cerr << "error: " << it->id << " is an unknown transaction field for New Folder\n";
	}
    }
  pathstr = Server->ServConf.FilesFolder;
  for (vector<string>::iterator it = filepath.begin(); it != filepath.end(); it++)
    {
      pathstr.append(it->c_str());
      pathstr.append("/");
    }
  CMyFile Object(pathstr+filename);
  return (Object.MkFolder());
}

bool CClServ::ActionMoveFile(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Move File\n";

  string		filename, pathstr, newpathstr;
  vector<string>	filepath, newpath;
  char			*data;

  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_FILE_NAME:
	  data = it->getdata();
	  filename.assign(data, it->size);
	  it->deldata();
	  break;
	case ID_FILE_PATH:
	  data = it->getdata();
	  filepath = Field202(data, it->size);
	  it->deldata();
	  //	  delete [] data;
	  break;
	case ID_FILE_NEWPATH:
	  data = it->getdata();
	  newpath = Field202(data, it->size);
	  //	  delete [] data;
	  it->deldata();
	  break;
	default:
	  std::cerr << "error: " << it->id << " is an unknown transaction field for Delete File\n";
	}
    }
  pathstr = Server->ServConf.FilesFolder;
  for (vector<string>::iterator it = filepath.begin(); it != filepath.end(); it++)
    {
      pathstr.append(it->c_str());
      pathstr.append("/");
    }
  newpathstr = Server->ServConf.FilesFolder;
  for (vector<string>::iterator it = newpath.begin(); it != newpath.end(); it++)
    {
      newpathstr.append(it->c_str());
      newpathstr.append("/");
    }
  CMyFile Object(pathstr+filename);
  return (Object.Move(newpathstr+filename));
}

void CClServ::ActionGetFileInfo(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Get File Info\n";

  string		filename;
  vector<string>	filepath;
  char			*data;

  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_FILE_NAME:
	  data = it->getdata();
	  filename.assign(data, it->size);
	  delete [] data;
	  break;
	case ID_FILE_PATH:
	  data = it->getdata();
	  filepath = Field202(data, it->size);
	  delete [] data;
	  break;
	default:
	  std::cerr << "error: " << it->id << " is an unknown transaction field for Delete File\n";
	}
    }
  c_tempstr = Server->ServConf.FilesFolder;
  for (vector<string>::iterator it = filepath.begin(); it != filepath.end(); it++)
    {
      c_tempstr.append(it->c_str());
      c_tempstr.append("/");
    }
  c_tempstr.append(filename);
  c_tempint = filename.length();
  if (DEBUG_L0)
    cout << " get file info for: " << c_tempstr << endl;
}

bool CClServ::ActionSetFileInfo(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Action Set File Info" << endl;
  
  string		filename, newname, comm, pathstr;
  vector<string>	filepath;
  char			*data;
  bool			rename, comment, ret;

  rename = false;
  comment = false;
  ret = true;
  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_FILE_NAME:
	  data = it->getdata();
	  filename.assign(data, it->size);
	  delete [] data;
	  break;
	case ID_FILE_PATH:
	  data = it->getdata();
	  filepath = Field202(data, it->size);
	  delete [] data;
	  break;
	case ID_FILE_NEWNAME:
	  data = it->getdata();
	  newname.assign(data, it->size);
	  delete [] data;
	  rename = true;
	  break;
	case ID_FILE_COMM:
	  data = it->getdata();
	  comm.assign(data, it->size);
	  delete [] data;
	  comment = true;
	  break;
	default:
	  std::cerr << "error: " << it->id << " is an unknown transaction field for Delete File\n";
	}
    }
  if (DEBUG_L0)
    cout << " set file info for: " << Server->ServConf.FilesFolder + filename << endl;
  
  if (rename)
    {
      pathstr = Server->ServConf.FilesFolder;
      for (vector<string>::iterator it = filepath.begin(); it != filepath.end(); it++)
	{
	  pathstr.append(it->c_str());
	  pathstr.append("/");
	}
      filename = pathstr + filename;
      newname = pathstr + newname;
      CMyFile Object(filename);
      ret = Object.Rename(newname);
    }
  if (comment)
    {
      cerr << "Sorry, comment files is not yet supported" << endl; 
    }
  return ret;
}  

void CClServ::ActionGetFileNameLst(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Action Get File Name List" << endl;

  char			*data;
  vector<string>	dirs;
  
  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_FILE_PATH:
	  data = it->getdata();
	  dirs = Field202(data, it->size);
	  delete [] data;
	  break;
	default:
	  cerr << "error: " << it->id << " is an unknown transaction field for Public Chat\n";
	}
    }
  c_tempstr = Server->ServConf.FilesFolder;
  for (vector<string>::iterator it = dirs.begin(); it != dirs.end(); it++)
    {
      c_tempstr.append(it->c_str());
      c_tempstr.append("/");
    }
}

bool CClServ::ActionInstantMessage(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Action Instant Message\n";

  char		*data;
  string	message;
  u_int16_t	uid;
  
  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_UID:
	  data = it->getdata();
	  memcpy(&uid, data, it->size);
	  it->deldata();
	  uid = ntohs(uid);
	  break;
	case ID_OPT:
	  break;
	case ID_DATA:
	  data = it->getdata();
	  message.assign(data, it->size);
	  it->deldata();
	  break;
	case ID_QUOT_MSG:
	  break;
	default:
	  std::cerr << "error: " << it->id << " is an unknown transaction field for Instant message\n";
	}
    }
  if (DEBUG_L1)
    {
      cout << " UID: " << uid << endl;
      cout << " Instant Message: " << message << endl;
    }
  if (Server->GetUser(uid))
    {
      c_tempint16 = uid;
      c_tempstr = message;
      ServMesFlag = 0;
      return true;
    }
  else
    return false;
}

bool CClServ::ActionDownloadFile(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Action Download File." << endl;

  u_int16_t		pflag;
  char			*data;
  string		filename;
  vector<string>	filepath;
  u_int32_t		resumept = 0;

  c_tempbool = false; /* init flag for preview or not */
  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_FILE_NAME:
	  data = it->getdata();
	  filename.assign(data, it->size);
	  it->deldata();
	  break;
	case ID_FILE_PATH:
	  data = it->getdata();
	  filepath = Field202(data, it->size);
	  it->deldata();
	  break;
	case ID_FILE_RESUME:
	  data = it->getdata();
	  resumept = Field203(data, it->size);
	  if (DEBUG_L1)
	    cout << " resume, current file size: " << resumept <<endl;	  
	  it->deldata();
	  break;
	case ID_FILE_TRANS_OPT:	  
	  data = it->getdata();
	  memcpy(&pflag, data, it->size);
	  it->deldata();
	  pflag = ntohs(pflag);
	  if (DEBUG_L1)
	    cout << " preview flag: " << pflag <<endl;
	  c_tempbool = true;
	  break;
	default:
	  cerr << "error: " << it->id << " is an unknown transaction field for Download file\n";
	}
    }  
  c_tempstr = Server->ServConf.FilesFolder;
  for (vector<string>::iterator it = filepath.begin(); it != filepath.end(); it++)
    {
      c_tempstr.append(it->c_str());
      c_tempstr.append("/");
    }
  c_tempint = filename.length();
  c_tempstr.append(filename.c_str());
  if (DEBUG_L0)
    cout << " send [" << c_tempstr <<"] to client." << endl;
  c_tempint32 = resumept;
  CMyFile Object(c_tempstr);
  return Object.Existenz();
}

void CClServ::ActionUploadFile(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Action Upload File." << endl;

  char			*data;
  string		filename;
  vector<string>	filepath;
  u_int32_t		tsizel;
  u_int16_t		tsizes;

  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      cout << "it->id: " << it->id << endl;
      switch (it->id)
    	{
    	case ID_FILE_NAME:
    	  data = it->getdata();
    	  filename.assign(data, it->size);
	  if (DEBUG_L1)
	    cout << " filename: " << filename << endl;
    	  delete [] data;
    	  break;
    	case ID_FILE_PATH:
    	  data = it->getdata();
    	  filepath = Field202(data, it->size);
    	  delete [] data;
    	  break;
    	case ID_FILE_TRANS_OPT:	  
    	  break;
    	case ID_TRANS_SIZE:
    	  data = it->getdata();
	  if (DEBUG_L1)
	    cout << " it->size: " << it->size << endl;
	  if (it->size == 4)
	    {
	      memcpy(&tsizel, data, it->size);
	      tsizel = ntohl(tsizel);
	      if (DEBUG_L1)
		cout << " file transfert size (long): " << tsizel << endl; 
	    }
	  else
	    {
	      memcpy(&tsizes, data, it->size);
	      tsizes = ntohs(tsizes);
	      if (DEBUG_L1)
		cout << " file transfert size (short): " << tsizes << endl; 
	    }
    	  delete [] data;
           break;
    	default:
    	  cerr << "error: " << it->id << " is an unknown transaction field for Download file\n";
    	}
    }  
  c_tempstr = Server->ServConf.FilesFolder;
  for (vector<string>::iterator it = filepath.begin(); it != filepath.end(); it++)
    {
      c_tempstr.append(it->c_str());
      c_tempstr.append("/");
    }
  c_tempint = filename.length();
  c_tempstr.append(filename.c_str());
  if (DEBUG_L0)
    cout << " recieve [" << c_tempstr << "] from client." << endl;
}

bool	CClServ::ActionDisconnectUser(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Disconnect User" << endl;
  u_int16_t	uid = 0;
  char		*data;
  
  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_UID:
	  data = it->getdata();
	  memcpy(&uid, data, it->size);
	  delete [] data;
	  uid = ntohs(uid);
	  break;
	case ID_OPT:
	  break;
	case ID_DATA:
	  break;
	default:
	  std::cerr << "error: " << it->id << " is an unknown transaction field for Set Client User Info\n";
	}
    }
  if (DEBUG_L1)
    cout << "disconnecting user " << uid << endl;
  if (uid == 0)
    return false;
  c_tempint16 = uid;
  return true;
}

bool	CClServ::ActionGetCliInfoTxt(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Get Client Info Text" << endl;
  
  u_int16_t	uid = 0;
  char		*data;

  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_UID:
	  data = it->getdata();
	  memcpy(&uid, data, it->size);
	  delete [] data;
	  uid = ntohs(uid);
	  break;
	default:
	  std::cerr << "error: " << it->id << " is an unknown transaction field for Set Client User Info\n";
	}
    }
  if (uid == 0)
    return false;
  c_tempint16 = uid;
  return true;  
}

void	CClServ::ActionGetAllAccounts(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Get All Accounts" << endl;
}

void	CClServ::ActionSaveAllAccounts(CTransac *transac)
{
  if (DEBUG_L0)
    cout << "Action step: Save All Accounts" << endl;
}

void	CClServ::ActionNewUser(CTransac *transac)
{
  char *data;
  string login, password, name, access;

  if (DEBUG_L0)
    cout << "Action step: New User" << endl;  
  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_USR_LOG:
	  data = it->getdata();
	  login.assign(get_HLCode(data, it->size));
	  it->deldata();
	  break;
	case ID_USR_PASS:
	  data = it->getdata();
	  password.assign(get_HLCode(data, it->size));
	  it->deldata();
	  break;
	case ID_USR_NAME:
	  data = it->getdata();
	  name.assign(data, it->size);
	  it->deldata();
	  break;
	case ID_USR_ACCESS:
	  for (vector<char>::iterator _it = it->vdata.begin(); _it != it->vdata.end(); _it++)
	    access.append(ctob((unsigned char)*_it));
	  break;
	default:
	  std::cerr << "error: " << it->id << " is an unknown transaction field for New User\n";
	}
    }
  if (DEBUG_L0)
    {
      cout << " login: " << login << " - password: " <<  password << " - name: " <<  name << endl;
      cout << " access: " << access << endl;
    }
  CUser NewUser(-1);
  NewUser.confname = name;
  NewUser.login = login;
  NewUser.password = password;
  for (int i = 0; i < 41; i++)
    {
      if (access.at(i) == '1')
	NewUser.access[i] = true;
      else
	NewUser.access[i] = false;
    }
  Server->AddUser(NewUser);
  NewUser.Serialize(ACCOUNTS + login);
}

bool	CClServ::ActionDelUser(CTransac *transac)
{
  char *data;
  string login;

  if (DEBUG_L0)
    cout << "Action step: Del User" << endl;  
  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_USR_LOG:
	  data = it->getdata();
	  login.assign(get_HLCode(data, it->size));
	  it->deldata();
	  break;
	default:
	  std::cerr << "error: " << it->id << " is an unknown transaction field for Del User\n";
	}
    }
  return (Server->DelUser(login));
}

void	CClServ::ActionGetUser(CTransac *transac)
{
  char *data;
  string login;
  
  if (DEBUG_L0)
    cout << "Action step: Get User" << endl;  
  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_USR_LOG:
	  data = it->getdata();
	  login.assign(data, it->size);
	  it->deldata();
	  break;
	default:
	  std::cerr << "error: " << it->id << " is an unknown transaction field for Get User\n";
	}
    }
  c_tempstr = login;
}

bool	CClServ::ActionSetUser(CTransac *transac)
{
  char *data;
  string login, password, name, access;

  if (DEBUG_L0)
    cout << "Action step: Set User" << endl;  
  for (list<CField>::iterator it = transac->field.begin();
       it != transac->field.end();
       it++)
    {
      switch (it->id)
	{
	case ID_USR_LOG:
	  data = it->getdata();
	  login.assign(get_HLCode(data, it->size));
	  it->deldata();
	  break;
	case ID_USR_PASS:
	  data = it->getdata();
	  password.assign(get_HLCode(data, it->size));
	  //password.assign(data, it->size);
	  it->deldata();
	  break;
	case ID_USR_NAME:
	  data = it->getdata();
	  name.assign(data, it->size);
	  it->deldata();
	  break;
	case ID_USR_ACCESS:
	  for (vector<char>::iterator _it = it->vdata.begin(); _it != it->vdata.end(); _it++)
	    access.append(ctob((unsigned char)*_it));
	  break;
	default:
	  std::cerr << "error: " << it->id << " is an unknown transaction field for New User\n";
	}
    }
  if (DEBUG_L0)
    {
      cout << " login: " << login << " - password: " << password << " - name: " <<  name << endl;
      cout << " access: " << access << endl;
    }
  c_tempstr.assign(login);
  CUser NewUser(-1);
  NewUser.confname = name;
  NewUser.login = login;
  NewUser.password = password;
  for (int i = 0; i < 41; i++)
    {
      if (access.at(i) == '1')
	NewUser.access[i] = true;
      else
	NewUser.access[i] = false;
    }
  return Server->SetUser(NewUser);
}

