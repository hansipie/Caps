#include "Serv.hpp"

u_int16_t	CServ::ActionAgreed(CUser *newUser)
{
  max_userId++; // faire un systeme de gestion des userID plus malin
  
  for (list<CUser>::iterator it2 = AccountsLst.begin();
       it2 != AccountsLst.end();
       it2++)
    {
      if (it2->login == newUser->login)
	{
	  it2->connected = true;
	  if (DEBUG_L1)
	    cout << "user " << newUser->login << " is connected" << endl;
	  break;
	}
    }
  return max_userId;
}

void		CServ::ActionBroadcast()
{
  for (vector<int>::iterator it = fds_used.begin(); it != fds_used.end(); it++)
    {
      int cur = *it;
      if (ClientsTable[cur]->User->connected)
	{
	  if (DEBUG_L1)
	    cout << "Setting Broadcast on " << cur << "." << endl;
	  for (list<CBuffer>::iterator it2 = BroadCastLst.begin(); it2 != BroadCastLst.end(); it2++)
	    {
	      char *BCast = it2->getbuff();
	      ClientsTable[cur]->SendList.push_back(CBuffer(BCast, it2->size));
	      it2->delbuff(); //delete BCast;
	    }
	  ClientsTable[cur]->set_send();
	}
    }
  BroadCastLst.clear();
}

void	CServ::ActionMsgTo()
{
  for (vector<int>::iterator it = fds_used.begin(); it != fds_used.end(); it++)
    {
      int cur = *it;
      //if ((ClientsTable[cur]->state & T_READ) || (ClientsTable[cur]->state & T_WRITE))
      if (ClientsTable[cur]->User->connected)
	{
	  for (list<CBufferTo>::iterator it2 = MsgToLst.begin(); it2 != MsgToLst.end(); it2++)
	    {
	      if (it2->to == ClientsTable[cur]->GetId())
	      {
		if (DEBUG_L1)
		  cout << "New instant message on " << cur << "." << endl;
		char *Msg = it2->getbuff();
		if (DEBUG_L1)
		  for (int i = 0; i < it2->size; i++)
		    printf("%#x ", Msg[i]);
		cout << endl;
		ClientsTable[cur]->SendList.push_back(CBuffer(Msg, it2->size));
		it2->delbuff();//delete Msg;
		ClientsTable[cur]->set_send();
	      }
	    }
	  cur++;
	}
    }
  MsgToLst.clear();
}

bool	CServ::ActionDisconnectUser(u_int16_t uid)
{
  for (vector<int>::iterator it = fds_used.begin(); it != fds_used.end(); it++)
    {
      int cur = *it;
      if (ClientsTable[cur]->User->userId == uid)
	{
	  ClientsTable[cur]->poison = true;
	  ClientsTable[cur]->set_send();
	  if (DEBUG_L1)
	    cout << "poisoning user " << uid << " on fd " << cur << endl;
	  return true;
	}
    }  
  return false;
}

list<CBuffer>	CServ::ActionGetUsrNameLst()
{
  if (DEBUG_L1)
    cout << "Server action: GetUsrNameLst" << endl;
  list<CBuffer>   ret;
  
  for (vector<int>::iterator it = fds_used.begin(); it != fds_used.end(); it++)
    {
      int cur = *it;
      
      //if ((ClientsTable[cur]->state & T_READ) || (ClientsTable[cur]->state & T_WRITE))
      if (ClientsTable[cur]->User->connected)
	{
	  u_int16_t size = ClientsTable[cur]->User->name.length();
	  u_int16_t htons_size = htons(size);
	  u_int16_t htons_iconId = htons(ClientsTable[cur]->User->iconId);
	  u_int16_t htons_userId = htons(ClientsTable[cur]->User->userId);
	  u_int16_t htons_userFlags = htons(ClientsTable[cur]->User->userFlags);
	  if (DEBUG_L1)
	    cout << "name: " << ClientsTable[cur]->User->name <<  "name size: " << size <<  " - icon id: " << ClientsTable[cur]->User->iconId << " - user id: " << ClientsTable[cur]->User->userId << " - flags: " << ClientsTable[cur]->User->userFlags << endl;
	  char *minibuff = new char[8 + size];
	  char *tmp;
	  tmp = minibuff;
	  /* user ID */
	  memcpy(tmp, &htons_userId, 2);
	  tmp += 2;
	  /* Icon Id */
	  memcpy(tmp, &htons_iconId, 2);
	  tmp += 2;
	  /* User flag */
	  memcpy(tmp, &htons_userFlags, 2);
	  tmp += 2;
	  /* User name size */
	  memcpy(tmp, &htons_size, 2);
	  tmp += 2;
	  /* User name */
	  memcpy(tmp, ClientsTable[cur]->User->name.c_str(), size);
	  CBuffer unamelst(minibuff, 8 + size);
	  ret.push_back(unamelst);
	  delete [] minibuff;
	}
    }
  return ret;
}

list<CBuffer>	CServ::ActionGetAllAccounts()
{
  if (DEBUG_L1)
    cout << "Server action: Get All Accounts" << endl;
  list<CBuffer>	ret;
  //CField	*Fname, *Flogin, *Fpass, *Faccess;
  CTransactions *transac;
  string	bits, data;
  char		*field;
  int		field_size;

  if (DEBUG_L2)
    cout << "_______________" << endl;
  for (list<CUser>::iterator it2 = AccountsLst.begin();
       it2 != AccountsLst.end();
       it2++)
    {
      transac = new CTransactions(0);
      transac->build_hdr(NO_TYPE, HDR_RPLY);
      transac->build_field_s(ID_USR_NAME, (char*)it2->confname.c_str());
      string HLlogin = get_HLCode((char *)it2->login.c_str(), it2->login.length());
      transac->build_field_s(ID_USR_LOG, (char*) HLlogin.c_str());
      transac->build_field_s(ID_USR_PASS, (char*)it2->password.c_str());
      bits.assign(64, '0');
      for (int i = 0; i < 41; i++)
	bits.replace(i, 1, (it2->access[i]?"1":"0"));
      if (DEBUG_L1)
	cout << bits << endl;
      vector<char> bitsfield = BitsField(bits);
      transac->build_field_b(ID_USR_ACCESS, bitsfield, 8);
      if (DEBUG_L2)
	transac->show_transac();
      transac->build_buff();
      field = transac->buff + HLHDR_SIZE;
      field_size = transac->buffsize - HLHDR_SIZE;
      CBuffer Buff(field, field_size);
      ret.push_back(Buff);
      delete transac;      
      if (DEBUG_L2)
	cout << "_______________" << endl;
    }
  return ret;
}

void CServ::AddUser(CUser NewUser)
{
  if (DEBUG_L1)
    cout << "Adding user " << NewUser.login << endl; 
  AccountsLst.push_back(NewUser);
}

bool CServ::DelUser(string login)
{
  for (list<CUser>::iterator it = AccountsLst.begin(); 
       it != AccountsLst.end(); it++)
    {
      if (it->login == login)
	{
	  if (DEBUG_L1)
	    cout << "Deleting user " << it->login << endl;
	  CMyFile file(ACCOUNTS+it->login);
	  file.Remove();
	  AccountsLst.erase(it);
	  return true;
	}
    }
  return false;
}

CUser *CServ::GetUser(u_int16_t uid)
{
  if (DEBUG_L1)
    cout << "Get User for UID " << uid;
  for (vector<int>::iterator it = fds_used.begin(); it != fds_used.end(); it++)
    {
      int cur = *it;
      if (ClientsTable[cur]->User->userId == uid)
	{
	  if (DEBUG_L1)
	    cout << " ... ok" << endl;
	  return (ClientsTable[cur]->User);
	}
    }
  if (DEBUG_L1)  
    cout << " ... failed" << endl;
  return NULL;
}

CUser *CServ::GetUser(string login)
{
  for (list<CUser>::iterator it = AccountsLst.begin(); 
       it != AccountsLst.end(); it++)
    {
      if (it->login == login)
	{
	  if (DEBUG_L1)
	    cout << "Getting user " << it->login << endl;
	  CUser *ret = &(*it);
	  return ret;
	}
    }
  return NULL;
}

bool CServ::SetUser(CUser User)
{
  for (list<CUser>::iterator it = AccountsLst.begin(); 
       it != AccountsLst.end(); it++)
    {
      if (it->login == User.login)
	{
	  if (DEBUG_L1)
	    cout << "setting user " << it->login << " in memory and on drive." << endl;
	  char *test = (char *)User.password.c_str();
	  if (test[0] == -1)
	    User.password = it->password;
	  AccountsLst.erase(it);
	  AccountsLst.push_back(User);
	  CMyFile file(ACCOUNTS+it->login);
	  file.Remove();
	  User.Serialize(ACCOUNTS+User.login);
	  return true;
	}
    }
  return false;  
}

bool CServ::UserRPM(u_int16_t uid)
{
  for (vector<int>::iterator it = fds_used.begin(); it != fds_used.end(); it++)
    {
      int cur = *it;
      if (ClientsTable[cur]->User->userId == uid)
	{
	  bool ret = ClientsTable[cur]->User->rpm;
	  if (DEBUG_L1)
	    cout << ClientsTable[cur]->User->name << (ret?"RPM : true":" RPM: false") << endl;
	  return ret;
	}
    }
  cerr << "error: uid " << uid << " not found" << endl;
  return false;
}

string CServ::UserName(u_int16_t uid)
{
  for (vector<int>::iterator it = fds_used.begin(); it != fds_used.end(); it++)
    {
      int cur = *it;
      if (ClientsTable[cur]->User->userId == uid)
	{
	  if (DEBUG_L1)
	    cout << ClientsTable[cur]->User->login << " is " << ClientsTable[cur]->User->name << endl;
	  return  ClientsTable[cur]->User->name;
	}
    }  
  cerr << "error: uid " << uid << " not found" << endl;
  return "";
}

string CServ::GetTypeStr(string file)
{
  if (DEBUG_L1)
    cout << " Get type of " << file << endl;

  string	ext, type("BINA");
  
  int pt = file.find_last_of(".");
  if (pt != -1)
    {
      int len = file.length();
      len = len - pt;
      ext = file.substr(pt, len);
      GetLower(ext);
      if (DEBUG_L1)
	cout << "  extension: " << ext << endl;
      map<string,string>::iterator it;
      if ((it = ExtToType.find(ext)) != ExtToType.end())
	{
	  if (DEBUG_L1)
	    cout << "  ext: " << it->first << " - type: " << it->second << endl;
	  type = it->second;
	}
      else
	if (DEBUG_L1)
	  cout << " not found" << endl;
    }
  return type;
}

string CServ::GetCreaStr(string file)
{
  if (DEBUG_L1)
    cout << " Get creator of " << file << endl;

  string	ext, crea("dosa");

  int pt = file.find_last_of(".");
  if (pt != -1)
    {
      int len = file.length();
      len = len - pt;
      ext = file.substr(pt, len);
      if (DEBUG_L1)
	cout << "  extension: " << ext << endl;

      map<string,string>::iterator it, it2;
      if ((it = ExtToType.find(ext)) != ExtToType.end()
	  && (it2 = TypeToCrea.find(it->second)) != TypeToCrea.end())
	{
	  if (DEBUG_L1)
	    cout << "  ext: " << it->first << " - creator: " << it2->second << endl;
	  crea = it2->second;
	}
      else
	if (DEBUG_L1)
	  cout << " not found" << endl;
    }
  return crea;
}

string CServ::GetUserIP(u_int16_t uid)
{
  if (DEBUG_L1)
    cout << "Get User IP for UID " << uid;
  for (vector<int>::iterator it = fds_used.begin(); it != fds_used.end(); it++)
    {
      int cur = *it;
      if (ClientsTable[cur]->User->userId == uid)
	{
	  if (DEBUG_L1)
	    cout << " ... ok" << endl;
	  return (ClientsTable[cur]->IPAdress);
	}
    }
  if (DEBUG_L1)  
    cout << " ... failed" << endl;
  return ("error: this user doesn't exist");
}

string CServ::GetUserInfo(CUser *user)
{
  if (DEBUG_L1)
    cout << "Get User Info for UID " << user->userId << endl;
  string ret;
  if (user == NULL)
    {
      if (DEBUG_L1)
	cout << " ...failed." << endl;
      return ("error: this user doesn't existe.");
    }
  else
    {
      ret.append(" - user name: ");
      ret.append(user->name);
      ret.append("\n - user login: ");
      ret.append(user->login);
      ret.append("\n - IP: " + GetUserIP(user->userId));
      if (DEBUG_L1)
	cout << "... done." << endl;
      return (ret);
    }  
}

void	CServ::SetNewUserAccess(string login)
{
  vector<int> ret;

  if (DEBUG_L1)
    cout << "Set New User Access for " << login << "." << endl;

  int cur;
  for (vector<int>::iterator it = fds_used.begin(); it != fds_used.end(); it++)
    {
      cur = *it;
      if ((ClientsTable[cur]->User->connected)
	  && (!ClientsTable[cur]->User->login.compare(login)))
	{
	  if (DEBUG_L1)
	    cout << " Changing User Access for " << ClientsTable[cur]->User->name << " under " << login << endl;
	  ClientsTable[cur]->MakeSetNewAccess(0);
	  ClientsTable[cur]->set_send();
	}
    }
}

