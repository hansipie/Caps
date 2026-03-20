#include "Serv.hpp"

CClServ::CClServ(CServ *ptr, int fd)
{
  if (DEBUG_L1)
    cout << "Constructeur CClServ" << endl;

  User = new CUser(fd);
  DwlUpl = NULL;
  Server = ptr;
  state = 0;
  state |= T_READ;
  step = HANDSHAKE;
  caps_cli = 0;
  c_tempfield = NULL;
  c_tempbuff = NULL;
  c_tempint16 = 0;
  c_tempbool = false;
  c_tempstr.clear();
  ServMesFlag = -1;
  IdTransac.clear();
  poison = false;
}

CClServ::~CClServ()
{
  if (DEBUG_L1)
    cout << "Destroying CClServ... " << endl;
  delete User;
  if (DwlUpl != NULL)
    delete DwlUpl;
  SendList.clear();
  RecvList.clear();
  TabPtrFunc.clear();
  IdTransac.clear();
  if (DEBUG_L1)
    cout << "... done." << endl;
}

// send a packet of bytes using a datagram
int CClServ::send_datagram(char *val, int len)
{
  int numbytes;
  
  numbytes=sendto(User->fd, val, len, 0,(struct sockaddr *) &dest_addr, sizeof(struct sockaddr));
  // numbytes = write(User->fd, val, len);
  if (numbytes == -1)
    {
      perror("sendto");
      return (-1);
    }
  if (DEBUG_L1)
    cout << "Server: sending datagram of " << numbytes << " bytes" << endl;
  if (DEBUG_L3)
    {
      print_ascii(val, numbytes);
      //print_hexa(val, numbytes);
    }
  return numbytes;
}

int CClServ::recv_datagram(char *val, int maxlen)
{
  int numbytes;
  socklen_t addr_len;

  addr_len = sizeof(struct sockaddr);
  numbytes=recvfrom(User->fd, val, maxlen, 0, (struct sockaddr *) &dest_addr, &addr_len);
  //numbytes = read(User->fd, val, maxlen);
  if (numbytes== -1)
    {
      perror("recvfrom");
      return (-1);
    }
  if (DEBUG_L1)
    cout << "Server: received datagram of " <<  numbytes << " bytes\n";
  if (DEBUG_L3)
    {      
      print_ascii(val, numbytes);
      //print_hexa(val, numbytes);
    }  
  return numbytes;
}


void CClServ::session(int io)
{
  if (DEBUG_L0 || DEBUG_L1 || DEBUG_L2 || DEBUG_L3)
    cout <<"-----------------------------------------------------------------------\n";
  switch (PortType)
    {
    case T_BP1:
      sessionBP1(io);
      break;
    case T_BP2:
      break; 
    case T_BP3:
      break;
    case T_BP:
    default:
      sessionBP(io);
    }
}

void CClServ::sessionBP(int io)
{
  switch (step)
    {
    case HANDSHAKE:
      if (io == SESSION_IN)
	Handshake();
      else
	cerr << "error: Handshake corruption\n";
      break;
    case ANY:
    default:
      if (io == SESSION_IN)
	{
	  if (!session_in())
	    return;
	  if (WhatToSend())
	    {
	      if (DEBUG_L1) 
		cout << "Server have to talk...\n";
	    }
	  else
	    {
	      if (DEBUG_L1)
		cout << "Server don't have to talk...\n";
	    }
	}
      else
	{
	  if (SendList.empty() && !poison)
	    {
	      cerr << "error: Session out error, send buffer is empty\n";
	      Suicide();
	    }
	  else
	    session_out();
	}
      if (DEBUG_L0 || DEBUG_L1 || DEBUG_L2 || DEBUG_L3)
	cout <<"-----------------------------------------------------------------------\n";
    }
}

void CClServ::sessionBP1(int io)
{
  int size;
  char *val;

  switch(step)
    {
    case DWL:
      if (DEBUG_L0)
	cout << "Client is downloading a file" << endl;
      if (DwlRequest())
	{
	  TabPtrFuncDU.push_back(&CClServ::DwlAction);
	  step = WAIT;
	}
      else
	step = END;
      set_send();
      break;
    case PRV:
      if (DEBUG_L0)
	cout << "Client is previewing a file" << endl;
      if (PrvRequest())
	{
	  TabPtrFuncDU.push_back(&CClServ::PrvAction);
	  step = WAIT;	  
	}
      else
	step = END;
      set_send();      
      break;
    case UPL:
      if (DEBUG_L0)
	cout << "Client is uploading a file" << endl;
      if (UplRequest())
	{
	  TabPtrFuncDU.push_back(&CClServ::UplAction);
	  step = WAIT;
	}
      else
	{
	  step = END;
	  set_send();
	}
      break;
    case WAIT:
      for (vector<DUFunc>::iterator it = TabPtrFuncDU.begin(); it != TabPtrFuncDU.end(); it++)
	{
	  if (!(this->**it)())
	    cerr << "Error while downloading/uploading file to client" << endl;
	}
      break;
    case PREVWAIT:
      Server->set_non_blocking(User->fd, true);
      size = recv_datagram(val, BUFFSIZE);
      if (size <= 0)
	{
	  if (DEBUG_L0)
	    cout << "exiting afer a preview" << endl;
	  step = END;
	  set_send();
	}
      Server->set_non_blocking(User->fd, false);
      break;
    case END:
      if (DEBUG_L0)
	cout << "End of transfert." << endl;
      TabPtrFuncDU.clear();
      EndDlwUpl();
      break;
    case HANDSHAKE:
    default:
      DUHandshake();
      break;
      if (DEBUG_L0 || DEBUG_L1 || DEBUG_L2 || DEBUG_L3)
	cout <<"-----------------------------------------------------------------------\n";      
    }
}

int CClServ::session_in()
{
  char *val, *tmpval;
  int size;
  int treatedsize = 0;

  if (DEBUG_L0)
    cout << "Client fd " << User->fd << " - input Session" <<endl;
  val = new char[BUFFSIZE];
  size = recv_datagram(val, BUFFSIZE);
  if (size <= 0)
    {
      Suicide();
      return (0);
    }
  else
    {
      while (treatedsize < size)
	{
	  tmpval = val + treatedsize;
	  CTransactions trame(tmpval, size);
	  RecvList.push_back(CTransacLst(trame.structure));
	  if (DEBUG_L2)  
	    trame.show_transac();
	  treatedsize += trame.structure.data_len + HLHDR_SIZE;
	  if (DEBUG_L1)	    
	    cout << "Buffer size: " << size <<" bytes - Treated: " << treatedsize << " bytes\n";
	}    
      for (list<CTransacLst>::iterator it = RecvList.begin(); it != RecvList.end(); it++)
	{
	  switch (analyse_rcvd(&it->structure))
	  {
	  case HDR_RPLY:
	    if (DEBUG_L0)
	      cout << "Server have to reply..." << endl;
	    break;
	  case DIE:
	    delete [] val;
	    if (DEBUG_L1)
	      cout << "Exiting zombi CClServ Class" << endl;
	    return (0);
	  case HDR_RQST:
	  default:
	    ;
	   }
	}
      RecvList.clear();
    }
  delete [] val;
  return (1);
}

void CClServ::session_out()
{
  if (DEBUG_L0)
    printf("Client fd %d - output Session\n", User->fd);  

  char *buff;
  for (list<CBuffer>::iterator it = SendList.begin(); it != SendList.end(); it++)
    {      
      buff = it->getbuff();
      send_datagram(buff, it->size);
      CTransactions trame(buff, it->size);
      if (DEBUG_L2)
	trame.show_transac();
      it->delbuff();
      IdTransac.pop_back();
    }
  if (!IdTransac.empty())
    {
      if (DEBUG_L1)
	{
	  cout << "warning: pending transactions (IdTransac tab not empty)." << endl;
	  cout << "or see known bugs from version 0.2a" << endl;
	}
    }
  else
    if (DEBUG_L1)
      cout << "output session ok - no transactions left." << endl;
  IdTransac.clear();
  set_recv();
  SendList.clear();
  if (poison)
    {
      if (DEBUG_L1)
	cout << "Client has been poisoned" << endl;
      Suicide();
      return;      
    }
}

void CClServ::set_recv()
{
  if (state & T_READ)
    return;
  state^=T_WRITE;
  state|=T_READ;
}

void CClServ::set_send()
{
  if (state & T_WRITE)
    return;
  state^=T_READ;
  state|=T_WRITE;
}

void CClServ::Suicide()
{ 
    MakeUserDelete(FreeId());
    Server->ActionBroadcast();
    Kill_fd();
}

void CClServ::Kill_fd()
{         
  Server->KillClient(User->fd);
  if (DEBUG_L0 || DEBUG_L1 || DEBUG_L2 || DEBUG_L3)
    cout << "############################-------------------------------------------\n";      
}

