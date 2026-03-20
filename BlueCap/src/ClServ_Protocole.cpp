#include "Serv.hpp"

int  CClServ::CliIsCaps(char *buff)
{
  char *tmp;
  
  tmp = &buff[4];
  if (strncmp(tmp, HAND_CAPS, 4) == 0)
    return (1);
  return (0);
}

int  CClServ::Handcorruption(char *buff)
{
  if ((strncmp(buff, HAND_DFLT, 4) == 0) && (buff[9] == 1)) 
    return (0);
  else
    {
      if (DEBUG_L0)
	cout << "Handshake corruption!!\n";
      return (1);
    }
}

void CClServ::Handshake()
{
  char valrcv[12];
  char valsnd[8];
  
  if (DEBUG_L0)
    cout << "handshake request from client" << endl;
  if (DEBUG_L1)
  cout << "setting User->fd: " << User->fd << endl; 
  if (recv_datagram(valrcv, 12) == 0)
    Suicide();
  else
    {
      if (CliIsCaps(valrcv))
	{
	  if (DEBUG_L0)
	    cout << " CAPS client\n";
	  caps_cli = true;
	}
      else
        {
	  if (DEBUG_L0)
	    cout << " Other client\n";
          caps_cli = false;
        }
      if (DEBUG_L0)
	cout << "handshake reply from server\n";
      strcpy(valsnd, HAND_DFLT);
      valsnd[4] = 0;
      valsnd[5] = 0;
      valsnd[6] = 0;
      if (Handcorruption(valrcv))
	{
	  valsnd[7] = 1;
	  Suicide();
	}
      else
	{
	  valsnd[7] = 0;	 
	  step = ANY;
	}
      send_datagram(valsnd, 8);
    }
}

int CClServ::FreeId()
{
  bool ok = false;
  int i = 0;

  if (DEBUG_L1)
    {
      cout << "used Ids: ";
      for (vector<int>::iterator it0 = IdTransac.begin(); 
	   it0 != IdTransac.end(); it0++)
	cout << *it0 << " ";
      cout << endl;
    }
  
  while (!ok)
    {
      i++;
      ok = true;
      for (vector<int>::iterator it = IdTransac.begin(); 
	   it != IdTransac.end(); it++)
	if (*it == i)
	  {
	    ok = false;
	    break;
	  }
    }
  if (DEBUG_L1)
    cout << "free Id: " << i << endl;
  IdTransac.push_back(i); 
  return i;
}

int CClServ::WhatToSend()
{
  int i = 0;
  if (DEBUG_L0)
    cout << "-WhatToSend------------------------------------------------------------\n";  
  if (!TabPtrFunc.empty())
    {
      for (vector<MakeTransFunc>::iterator it = TabPtrFunc.begin(); it != TabPtrFunc.end(); it++)
	(this->**it)(FreeId());
      TabPtrFunc.clear();
      set_send();
      i = 1;
    }
  if (!Server->MsgToLst.empty())
    {
      Server->ActionMsgTo();
      i = 1;
    }
  if (!Server->BroadCastLst.empty())
    {
      Server->ActionBroadcast();
      i = 1;
    }
  return(i);
}

int CClServ::analyse_rcvd(CTransac *transac)
{
  if (DEBUG_L1)
    std::cout << "transaction analysis\n";
  int	Id;
  
  Id = transac->id;
  if (DEBUG_L1)
    {
      cout << "new Id: " << Id << endl;
      cout << "used Ids: ";
      for (vector<int>::iterator it0 = IdTransac.begin(); 
	   it0 != IdTransac.end(); it0++)
	cout << *it0 << " ";
      cout << endl;
    }
  for (vector<int>::iterator it = IdTransac.begin(); 
       it != IdTransac.end(); it++)
    {
      if (*it == Id)
	{
	  c_error = "error: Session error, invalid IdTransac.";
	  std::cerr << c_error << endl;
	  MakeError(Id);
	  set_send();
	  return (HDR_RPLY);
	}
    }
  if (DEBUG_L1)
    cout << "Id " << Id << " is ok." << endl;
  IdTransac.push_back(Id);
  if (User->connected || ((int)transac->grbg[1] == LOGIN) 
      || ((int)transac->grbg[1] == AGREED)
      || ((int)transac->grbg[1] == GET_UNAME_LST))
    switch (ReplyType(transac, Id))
      {
      case HDR_RPLY:
	return HDR_RPLY;
      case HDR_RQST:
	return HDR_RQST;
      case DIE:
	return DIE;
      case HL_ERROR:
      default:
	;
      }
  cerr << "error: User not connected, can't request trasaction " << (int)transac->grbg[1] << endl;
  c_error = "error: not connected.";
  MakeError(Id);
  set_send();
  return (HDR_RPLY);
}

int CClServ::ReplyType(CTransac *transac, int Id)
{  
  int  noreply[6] = {HL_ERROR, SEND_CHAT, INVIT_TO_CHAT, LEAVE_CHAT, 
		     SET_CHAT_SUBJECT, SET_CL_USR_INF};
  int  shortreply[18] = {OLD_POST, SEND_MSG, DISC_USER, AGREED, DEL_FILE, 
			 NEW_FOLD, SET_FILEI, MOVE_FILE, MK_FILE_ALIAS, 
			 NEW_USER, DEL_USER, SET_USER, USER_BROADCAST, 
			 DEL_NEWS_IT, NEW_NEWS_FOLD, NEW_NEWS_CAT,
			 POST_NEWS_ART_DATA, DEL_NEWS_ART};
  int  longreply[19] = {GET_MSG, LOGIN, INVIT_TO_NEW_CHAT, JOIN_CHAT, 
			GET_FNAME_LST, DWNL_FILE, UPL_FILE, GET_FILEI, 
			DWNL_FOLD, DWNL_BAN, UPL_FOLD, GET_UNAME_LST, 
			GET_CL_INF_TXT, GET_USER, GET_NEWS_CAT_NAME_LST, 
			GET_NEWS_ART_NAME_LST, GET_NEWS_ART_DATA, 
			GET_ALL_ACCOUNTS, SAVE_ALL_ACCOUNTS};
  int  i;
  
  for (i = 0; i < 6; i++)
    if (noreply[i] == (int)transac->grbg[1])
      {
	if (DEBUG_L1)
	  cout << "no reply" << endl;
	if (NoReply(transac, Id) == HDR_RPLY)
	  return (HDR_RPLY);
	else
	  return (HDR_RQST);
      }
  for (i = 0; i < 18; i++)
    if (shortreply[i] == (int)transac->grbg[1])
      {
	if (DEBUG_L1)
	  cout << "short reply" << endl;
	ShortReply(transac, Id);
	return (HDR_RPLY);
      }
  for (i = 0; i < 19; i++)
    if (longreply[i] == (int)transac->grbg[1])
      {
	if (DEBUG_L1)
	  cout << "long reply" << endl;
	switch (LongReply(transac, Id))
	  {
	  case DIE:   
	    return (DIE);
	  case HDR_RPLY: 
	  default:
	    return (HDR_RPLY);
	  }
      }
  cerr << "error: " << transac->grbg[1] << " is an unknown client command\n";  
  return (HL_ERROR);
}

int CClServ::NoReply(CTransac *transac, int Id)
{
  int type = (int)transac->grbg[1];
  if (DEBUG_L0 || DEBUG_L1 || DEBUG_L2)
    cout << "-----------------------------------------------------------------------\n";
  switch (type)
    {
    case HL_ERROR:			/*100*/
      /* pas de test des droits */
      if (DEBUG_L0)
	cout << "error from client" << endl;
      break;
    case SEND_CHAT:		/*105*/
      if (User->access[10])
	{
	  ActionPublicChat(transac);
	  MakePublicChat(FreeId());
	}
      else
	{
	  ReturnRightsError(Id, type);
	  set_send();
	  return (HDR_RPLY);
	}     
      break;
    case SET_CL_USR_INF:	/*304*/
      /* pas de test des droits */
      ActionSetClUsrInf(transac);
      if (DEBUG_L0)
	cout << "Set user" << User->name << " with icon " << User->iconId << endl;
      TabPtrFunc.push_back(&CClServ::MakeUserChange);
      break;
    case INVIT_TO_CHAT:		/*113*/
    case LEAVE_CHAT:		/*116*/
    case SET_CHAT_SUBJECT:	/*120*/
    default:
      cerr << "error: NoReply transaction " << type << " is a not implemented" << endl;
      c_error = "error: unknown transaction.";
      MakeError(Id);
      set_send();
      return (HDR_RPLY);
    }
  return (HDR_RQST);
}

int CClServ::ShortReply(CTransac *transac, int Id)
{
  int type = (int)transac->grbg[1];
  
  if (DEBUG_L0 || DEBUG_L1 || DEBUG_L2)
    cout << "-----------------------------------------------------------------------\n";
  switch (type)
    {
    case OLD_POST:	/*103*/
      if (User->access[21])
	{
	  ActionOldPostNews(transac); 
	  TabPtrFunc.push_back(&CClServ::MakeNewMessage);
	}
      else
	{
	  ReturnRightsError(Id, type);
	  set_send();
	  return HDR_RPLY;
	}
	break;
    case SEND_MSG:	/*108*/
      if (!ActionInstantMessage(transac))
	{
	  if (DEBUG_L1)
	    cerr << "error: ActionInstantMessage has return an error." << endl;
	  c_error = "This user doesn't exist.";
	  MakeError(Id);
	  set_send();
	  return HDR_RPLY;
	} 	
      MakeServerMessage(FreeId());
      break;
    case DISC_USER:	/*110*/
      if (User->access[22])
	{
	  if (!ActionDisconnectUser(transac))
	    {
	      if (DEBUG_L1)
		cerr << "error: ActionDisconnectUser has return an error." << endl;
	      c_error = "This user doesn't exist.";
	      MakeError(Id);
	      set_send();
	      return HDR_RPLY;
	    }
	  if (!Server->ActionDisconnectUser(c_tempint16))
	    {
	      if (DEBUG_L1)  
		cerr << "error: Server->ActionDisconnectUser has return an error." << endl;
	      c_error = "This user doesn't exist.";
	      MakeError(Id);
	      set_send();
	      return HDR_RPLY;
	    }	
	  c_tempint16 = 0;
	  //
	  // exemple de Disconnect Msg
	  //
	  //c_tempint16 = ??;
	  //c_tempstr.assign("casse toi");
	  //MakeDisconnectMsg(FreeId());
	  //
	}
      else
	ReturnRightsError(Id, type);
      break;
    case AGREED:	/*121*/
      /* pas de test des droits */
      ActionAgreed(transac);
      if (DEBUG_L0)
	cout << "Agreed from " << User->login << ":" << User->password 
	     << "(v." << User->version << ") as " << User->name << "(" 
	     << User->userId << ") with icon " << User->iconId 
	     << " connected " << User->connected << endl ;
      MakeUserAccess(FreeId());  
      break;
    case DEL_FILE:	/*204*/
      if (User->access[0] || User->access[6])
	{
	  if (!ActionDelFile(transac))
	    {
	      if (DEBUG_L1)
		cerr << "error: ActionDelFile has return an error." << endl;
	      c_error = "error while deleting file.";
	      MakeError(Id);
	      set_send();
	      return (HDR_RPLY);
	    }
	}
      else
	{
	  ReturnRightsError(Id, type);
	  set_send();
	  return HDR_RPLY;
	}      
      break;
    case NEW_FOLD:	/*205*/
      if (User->access[5])
	{
	  if (!ActionNewFolder(transac))
	    {
	      if (DEBUG_L1)
		cerr << "error: ActionNewFolder has return an error." << endl;
	      c_error = "error while creating folder.";
	      MakeError(Id);
	      set_send();
	      return (HDR_RPLY);
	    }
	}
      else
	{
	  ReturnRightsError(Id, type);
	  set_send();
	  return (HDR_RPLY);
	}
      break;
    case SET_FILEI:	/*207*/
      if (User->access[28] || User->access[29])
	{
	  if (!ActionSetFileInfo(transac))
	    {
	      if (DEBUG_L1)
		cerr << "error: ActionSetFileInfo has return an error." << endl;
	      c_error = "error while setting file infos.";
	      MakeError(Id);
	      set_send();
	      return (HDR_RPLY);
	    }
	}
      else
	{
	  ReturnRightsError(Id, type);
	  set_send();
	  return (HDR_RPLY);
	}
      break;
    case MOVE_FILE:	/*208*/
      if (User->access[4])
	{
	  if (!ActionMoveFile(transac))
	    {
	      if (DEBUG_L1)
		cerr << "error: ActionMoveFile has return an error." << endl;
	      c_error = "error while moving file.";
	      MakeError(Id);
	      set_send();
	      return (HDR_RPLY);
	    }
	}
      else
	{
	  ReturnRightsError(Id, type);
	  set_send();
	  return (HDR_RPLY);
	}
      break;
    case NEW_USER:	/*350*/
      if (User->access[14])
	ActionNewUser(transac);
      else
	{
	  ReturnRightsError(Id, type);
	  set_send();
	  return (HDR_RPLY);
	}
      break;
    case DEL_USER:	/*351*/
      if (User->access[15])
	{
	  if (!ActionDelUser(transac))
	    {
	      if (DEBUG_L1)
		cerr << "error: ActionDelUser has return an error." << endl;
	      MakeError(Id);
	      set_send();
	      return (HDR_RPLY);	  
	    }
	}
      else
	{
	  ReturnRightsError(Id, type);
	  set_send();
	  return (HDR_RPLY);
	}
      break;
    case SET_USER:	/*353*/
      if (User->access[17])
	{
	  if (!ActionSetUser(transac))
	    {
	      if (DEBUG_L1)
		cerr << "error: ActionSetUser has return an error." << endl;
	      c_error = "error while setting user.";
	      MakeError(Id);
	      set_send();
	      return (HDR_RPLY);	  
	    }
	  Server->SetNewUserAccess(c_tempstr);
	}
      else
	{
	  ReturnRightsError(Id, type);
	  set_send();
	  return (HDR_RPLY);
	}      
      break;
    case USER_BROADCAST:	/*355*/
      if (User->access[32])
	{
	  ActionBroadcast(transac);
	  if (!MakeBroadCast(FreeId()))
	    {
	      if (DEBUG_L1)
		cerr << "error: MakeBroadCast has return an error." << endl;
	      c_error = "Nothing to broadcast.";
	      MakeError(Id);
	      set_send();
	      return (HDR_RPLY);	  
	    }
	}
      else
	{
	  ReturnRightsError(Id, type);
	  set_send();
	  return (HDR_RPLY);
	}
      break;
    case MK_FILE_ALIAS:	/*209*/
    case DEL_NEWS_IT:	/*380*/
    case NEW_NEWS_CAT:	/*381*/ 
    case GET_NEWS_ART_DATA:	/*382*/ 
    case POST_NEWS_ART_DATA:	/*410*/ 
    case DEL_NEWS_ART:	/*411*/
    default:
      cerr << "error: ShortReply transaction " << type << " is not implemented" << endl;
      c_error = "error: unknown transaction.";
      MakeError(Id);
      set_send();
      return (HDR_RPLY);
    }
  MakeShortReply(Id);
  set_send();
  return (HDR_RPLY);
}

int CClServ::LongReply(CTransac *transac, int Id)
{
  int type = (int)transac->grbg[1];
  if (DEBUG_L0 || DEBUG_L1 || DEBUG_L2)
    cout << "-----------------------------------------------------------------------\n";
  
  switch (type)
    {
    case GET_MSG:		/*101*/
      /* pas de test des droits */
      MakeGetMessageReply(Id);
      break;
    case LOGIN:			/*107*/
      /* pas de test des droits */
      if (!ActionLogin(transac))
	return(DIE);
      if (DEBUG_L0)
	cout << "login request from " << User->login << ":" << User->password <<"(v." << User->version << ")\n";
      MakeLoginReply(Id);
      TabPtrFunc.push_back(&CClServ::MakeAgreement);
      break;
    case GET_FNAME_LST:		/*200*/
      /* pas de test des droits */
      ActionGetFileNameLst(transac);
      MakeGetFileNameLstReply(Id);
      break;
    case DWNL_FILE:		/*202*/
      if (User->access[2])
	{
	  if (ActionDownloadFile(transac))
	    MakeDownloadFileReply(Id);
	  else
	    { 
	      c_error = "This file dosn't exist.";
	      MakeError(Id);
	    }	  
	}
      else
	ReturnRightsError(Id, type);
      break;
    case UPL_FILE:		/*203*/
      if (User->access[1])
	{
	  ActionUploadFile(transac);
	  MakeUploadFileReply(Id);
	}
      else
	ReturnRightsError(Id, type);
      break;
    case GET_FILEI:		/*206*/
      /* pas de test des droits */
      ActionGetFileInfo(transac);
      if (!MakeGetFileInfoReply(Id))
      {
	c_error = "This file doesn't exist.";
	MakeError(Id);
      }
      break;
    case GET_UNAME_LST:		/*300*/
      /* pas de test des droits */
      MakeGetUsrNameLstReply(Id);
      break;
    case GET_CL_INF_TXT:	/*303*/
      if (User->access[24])
	{
	  if (!ActionGetCliInfoTxt(transac))
	    goto cliinttxterror;
	  if (!MakeGetCliInfoTxtReply(Id))
	    goto cliinttxterror;
	  else
	    goto cliinttxtexit;
	cliinttxterror:
	  c_error = "This user doesn't exist.";
	  MakeError(Id);
	cliinttxtexit:
	  ;
	} 
      else
	ReturnRightsError(Id, type);
      break;
    case GET_ALL_ACCOUNTS:		/*348*/
      ActionGetAllAccounts(transac);
      MakeGetAllAccountsReply(Id);
      break;
    case SAVE_ALL_ACCOUNTS:		/*349*/
      ActionSaveAllAccounts(transac);
      MakeSaveAllAccountsReply(Id);
      cerr << "error: LongReply transaction " << type << " is a not supported" << endl;
      c_error = "error: unknown transaction.";
      MakeError(Id);
      break;
    case GET_USER:			/*352*/
      ActionGetUser(transac);
      if (!MakeGetUserReply(Id))
	{
	  c_error = "error while getting user.";
	  MakeError(Id);
	}
      break;
    case INVIT_TO_NEW_CHAT:	/*112*/
    case JOIN_CHAT:		/*115*/
    case DWNL_FOLD:		/*210*/
    case DWNL_BAN:		/*212*/
    case UPL_FOLD:		/*213*/
    case GET_NEWS_CAT_NAME_LST:	/*370*/
    case GET_NEWS_ART_NAME_LST:	/*371*/
    case GET_NEWS_ART_DATA:	/*400*/
    default:      
      cerr << "error: LongReply transaction " << type << " is a not implemented" << endl;
      c_error = "error: unknown transaction.";
      MakeError(Id);
    } 
  set_send();
  return (HDR_RPLY);
}

