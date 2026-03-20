#include "Serv.hpp"

bool  CClServ::DUHandcorruption(char *buff)
{
  if (strncmp(buff, DUHAND_DFLT, 4) == 0) 
    {
      if (DEBUG_L0)
	cout << "download/upload Handshake is ok.\n";
      caps_cli = false;
      return false;
    }
  if (DEBUG_L0)
    cout << "download/upload Handshake corruption!!\n";
  return true;
}

int CClServ::readref(char *valrcv)
{
  u_int32_t ref;
  
  memcpy(&ref, &valrcv[4], 4);
  ref = ntohl(ref);
  if (DEBUG_L1)
    cout << " transfer ref: " << ref <<endl;
  return ref;
}

int CClServ::readsize(char *valrcv)
{
  u_int32_t size;
  
  memcpy(&size, &valrcv[8], 4);
  size = ntohl(size);
  if (DEBUG_L1)
    cout << " transfer size: " << size <<endl;
  return size;
}

void CClServ::DUHandshake()
{
  char	valrcv[12];
  
  if (DEBUG_L0)
    cout << "download/upload handshake request from client" << endl;
  if (DEBUG_L1)
    cout << "setting User->fd: " << User->fd << endl; 
  if (recv_datagram(valrcv, 16) == 0)
    Suicide();
  else
    {
      if (DUHandcorruption(valrcv))
	{
	  Suicide();
	  return;
	}
      else
	{
	  m_ref = readref(valrcv);
	  readsize(valrcv);
	  map<int,bool>::iterator it3;
	  if ((it3 = Server->RefTableCaps.find(m_ref)) != Server->RefTableCaps.end())
	    {
	      caps_cli = it3->second;
	      string CFlag((it3->second)?"True":"False");
	      if (DEBUG_L1)
		cout << " ref: " << it3->first << " - caps flag: " << CFlag << endl;
	    } 
	  else
	    {
	      if (DEBUG_L1)
		cout << " unknown transfert reference " << m_ref <<endl;
	      Suicide();
	      return;
	    }
	  map<int,int>::iterator it;
	  it = Server->RefTable.find(m_ref);
	  string AFlag;
	  switch(it->second)
	    {
	    case DWL:
	      AFlag = "Download";
	      step = DWL;
	      set_send();
	      break;
	    case PRV:
	      AFlag = "Preview";
	      step = PRV;
	      set_send();
	      break;
	    case UPL:
	      AFlag = "Upload";
	      step = UPL;
	      break;
	    default:
	      if (DEBUG_L1)
		cout << " unknown transfert type" << endl;
	      Suicide();
	      return;
	    }
	  if (DEBUG_L1)
	    cout << " - action flag: " << AFlag << endl; 
	  map<int,string>::iterator it2;
	  it2 = Server->RefTableFile.find(m_ref);
	  if (DEBUG_L1)
	    cout << " - file: " << it2->second << endl;
	  filefortransfert =  it2->second;
	  map<int,u_int32_t>::iterator it4;
	  it4 = Server->RefTableResume.find(m_ref);
	  if (DEBUG_L1)
	    cout << " - current file size: " << it4->second << endl;
	  m_resumept =  it4->second;
	}
    }
}

bool CClServ::DwlRequest()
{
  if (!filefortransfert.empty())
    {
      if (DEBUG_L1)
	cout << "file: " << filefortransfert << endl;
      CFlFileObj *NewFFO = new CFlFileObj();
      NewFFO->InitFiletype(Server->ExtToType, Server->TypeToCrea);
      NewFFO->make_FFO(filefortransfert);
      if (DEBUG_L2)
	NewFFO->show_FFO();
      c_tempint32 = NewFFO->get_FileSize();
      if (DEBUG_L1)
	cout << "FFO_buff_size: " << NewFFO->FFO_buff_size << endl;
      send_datagram(NewFFO->FFO_buff, NewFFO->FFO_buff_size);
      delete NewFFO;
      DwlUpl = new CDwlUpl(filefortransfert, "read");
      filefortransfert.clear();
      if (DwlUpl->error)
	return false;
      return true;
    }
  else
    {
      cerr << "error: no file to send" << endl;
      return false;
    } 
}

bool CClServ::DwlAction()
{
  int size;
  bool ret ;
  if (DEBUG_L1)
    cout << "DwlAction on " << DwlUpl->file << endl;
  //DwlUpl->MoveTo(m_resumept);
  size = DwlUpl->GetBuff();
  DwlUpl->total += size;
  if (DEBUG_L1)
    cout << " read: " << size <<" - total: "<< DwlUpl->total << endl;
  if (size <= 0)
    {
      if (((u_int32_t)DwlUpl->total != c_tempint32) && (c_tempint32 != 0))
	{
	  cerr << "file transfert is not complete" << endl;
	  ret = false;
	  step = END;
 	}
      else
	{
	  if (DEBUG_L0)
	    cout << "file transfert is complete" << endl;
	  ret = true;
	  step = END;
	}
      if (DwlUpl->error) 
	{
	  cerr << "DwlUpl error" << endl;
	  ret = false;
	  step = END;
	}
      c_tempint32 = 0;
    }
  else
    {
      if (send_datagram(DwlUpl->buffer, size) == -1)
	{
	  ret = false;
	  set_send();
	  step = END;
	}
      else
	ret = true;
    }
  DwlUpl->DelBuff();
  return ret;
}

bool CClServ::PrvRequest()
{
  if (!filefortransfert.empty())
    {
      if (DEBUG_L1)
	cout << " file: " << filefortransfert << endl << " making preview ... " << endl;
      ifstream orgfile (filefortransfert.c_str(), ios::in|ios::binary|ios::ate);
      int orgsize = orgfile.tellg();
      orgfile.close();
      if (DEBUG_L1)
	cout << " original size: " << orgsize << endl;
      if (orgsize <= 0)
	{
	  cerr << "error: empty file" << endl;
	  set_send();
	  step = END;
	  return false;
	}
      
      if (Is_movie(GetExtension(filefortransfert)) && caps_cli)
	{
	  if (fork() == 0)
	    {
	      string commandl("-vvv --noaudio ");
	      commandl.append(filefortransfert);
	      commandl.append(" --sout udp://");
	      commandl.append(IPAdress);
	      commandl.append(" --ttl 12 vlc:quit");
	      if (DEBUG_L0 || DEBUG_L1)
		{
		  cout << " Using vlc for the preview" << endl;
		  cout << "  " << Server->ServConf.Videolan << " " << commandl << endl;
		}
	      string vidip(IPAdress);
	      string sout("udp://" + vidip);
	      if (execle(Server->ServConf.Videolan.c_str(), 
			 "-q", 
			 "--noaudio", 
			 filefortransfert.c_str(), 
			 "--sout", sout.c_str(), 
			 "--ttl", "12", 
			 "vlc:quit", 
			 NULL, Server->env)
		  == -1)
		perror("execle");
	      exit(0);
	    }
	  filefortransfert.assign(VLCPICT);
	}
      string ext;
      if (caps_cli)
	ext = ".jpg";
      else
	ext = GetExtension(filefortransfert);
      
      CMyFile Object(filefortransfert);
      Object.Make_preview(ext);
      if (Object.Existenz())
	{
	  DwlUpl = new CDwlUpl(Object.File, "read");
	  DwlUpl->f_delete = true;
	}
      else
	DwlUpl = new CDwlUpl(filefortransfert, "read");
      filefortransfert.clear();
      c_tempint32 = 0;
      if (DwlUpl->error)
	return false;
      return true;
    }
  else
    {
      cerr << "error: no file to send" << endl;
      set_send();
      step = END;
      return false;
    } 
}

bool CClServ::PrvAction()
{
  if (DEBUG_L1)
    cout << "Preview action throught ";
  bool ret = DwlAction();
  if (step == END)
    {
      if (!caps_cli)
	{
	  step = PREVWAIT;
	  set_recv();
	}
      CMyFile Object(DwlUpl->file);
      if (DwlUpl->f_delete)
	{
	  if (DEBUG_L1)
	    cout << "Deleting preview..." << endl;
	  Object.Remove();
	}
    }
  return ret;
}

bool CClServ::UplRequest()
{
  if (!filefortransfert.empty())
    {
      if (DEBUG_L1)
	cout << "file: " << filefortransfert << endl;;

      DwlUpl = new CDwlUpl(filefortransfert, "write");
      //Server->set_non_blocking(User->fd, true);
      bool ret;
      char *valrcv = new char [BUFFSIZE];
      char *tmprcv = valrcv;

      int size = recv_datagram(valrcv, BUFFSIZE);
      if (DEBUG_L1)
	cout << "first recv = " << size << endl;
      if (size > 0)
	{
	  int ffo_size = get_FFO(valrcv, size);
	  if (DEBUG_L1)
	    cout << " ffo size = " << ffo_size << endl;
	  valrcv += ffo_size;
	  size = size - ffo_size;
	  DwlUpl->PutBuff(valrcv, size);
	  DwlUpl->total += size;
	  if (DEBUG_L1)
	    cout << " write: " << size <<" - total: "<< DwlUpl->total << endl;
	  step = WAIT;
	  ret = true;
	}
      else
	{
	  cerr << "first recv is null" << endl;
	  step = END;
	  set_send();
	  ret = false;
	}
      delete [] tmprcv;
      //Server->set_non_blocking(User->fd, false);
      filefortransfert.clear();
      return ret;
    }
  else
    {
      cerr << "error: no file to recieve" << endl;
      step = END;
      set_send();
      return false;
    }
}

bool CClServ::UplAction()
{
  bool ret;
  if (DEBUG_L1)
    cout << "UplAction on " << DwlUpl->file << endl;
  char *valrcv = new char [BUFFSIZE];
  //Server->set_non_blocking(User->fd, true);
  int size = recv_datagram(valrcv, BUFFSIZE);
  switch (size)
    {
    case 0:
      DwlUpl->CloseFile();
      step = END;
      set_send();
      ret = true;
      break;
    case -1:
      ret = false;
      break;
    default:      
      DwlUpl->PutBuff(valrcv, size);
      DwlUpl->total += size;
      if (DEBUG_L1)
	cout << " write: " << size <<" - total: " << DwlUpl->total << endl;
      ret = true;
    }
  //Server->set_non_blocking(User->fd, false);
  delete [] valrcv;
  return ret;
}

void CClServ::EndDlwUpl()
{
//     char *val = new char[BUFFSIZE];
//     int size = recv_datagram(val, BUFFSIZE);
//     if (size <= 0)

//       cerr << "error: Server has recieved something but he was in END Dl/Ul state" << endl;
  map<int,bool>::iterator it3;
  it3 = Server->RefTableCaps.find(m_ref);
  Server->RefTableCaps.erase(it3);
  map<int,int>::iterator it;
  it = Server->RefTable.find(m_ref);
  Server->RefTable.erase(it);
  map<int,string>::iterator it2;
  it2 = Server->RefTableFile.find(m_ref);
  if (DEBUG_L1)
    cout << "Cleannig RefTables: " << it2->first << " - " << it2->second <<endl;
  Server->RefTableFile.erase(it2);
  set_recv();
  Kill_fd();
}    

int CClServ::get_FFO(char *buff, int size)
{
  int ret;

  CFlFileObj *NewFFO = new CFlFileObj(buff, size);
  NewFFO->InitFiletype(Server->ExtToType, Server->TypeToCrea);
  NewFFO->parse_FFO();
  NewFFO->show_FFO();
  ret = NewFFO->get_FFO_size();
  c_tempint32 = NewFFO->get_FileSize();
  delete NewFFO;
  return ret;
}



