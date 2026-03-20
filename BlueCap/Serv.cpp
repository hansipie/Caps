#include "Serv.hpp"

CServ::CServ()
{
  if (DEBUG_L1)
    cout << "Constructeur Class CServ" << endl;
  max_userId = 0;
  InitUsers();
  InitServer();
  InitFilesTypes();
  baseport = ServConf.Port;
#ifdef __WIN32__  
  int iResult = WSAStartup(MAKEWORD(2,2), &m_wsaData);
  if (iResult != NO_ERROR)
    cerr << "Error at WSAStartup()" << endl;
#endif
  Socket();
  Bind();
  Listen();
}

CServ::~CServ()
{
  if (DEBUG_L1)
    cout << "Destructeur CServ" << endl;
}

void CServ::InitFilesTypes()
{
  GetFilesType();
}

void CServ::InitUsers()
{
  GetAccounts("./accounts/");
  if (DEBUG_L1)
    {
      cout << "Accounts list:" << endl;
      for (list<CUser>::iterator it2 = AccountsLst.begin();
	   it2 != AccountsLst.end();
	   it2++)
	{
	  cout << "[" << it2->confname << "][" << it2->login << "][" << it2->password << "][" << it2->iconId << "]" << endl;
	  for (int i=0; i < 41; i++)
	    cout << it2->access[i];
	  cout << endl;
	  if (it2->connected)
	    cout << "connected" << endl;
	  else
	    cout << "not connected" << endl;
	}
    }
}

void CServ::InitServer()
{
  char buffer[256];
  string line;

  ServConf.ServerName = DLFT_NAME;
  ServConf.Port = DLFT_PORT;
  ServConf.Agreement = DLFT_AGREEM;
  ServConf.FilesFolder = DLFT_FILESFLD;
  ServConf.FilesType = DLFT_FILEST;
  ServConf.Videolan = "";

  ifstream conf_file (CONF_FILE);
  if (!conf_file.is_open())
    { 
      cerr << "Error opening configuration file:" << endl; 
      exit(1); 
    }
  while (!conf_file.eof())
    {
      conf_file.getline(buffer, 256, '\n');
      line = buffer;
      ParseConfLine(line);
    }
  conf_file.close();
}

void CServ::Socket()
{  
  bool dum = true; /*dummy variable*/
  if (DEBUG_L1)  
    cout << "Server: opening socket on port = " << baseport << endl;
  baseport1 = baseport+1;
  if (DEBUG_L1)
    cout << "Server: opening socket on port = " << baseport1 << endl;
  
  if ((BPsockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET ) 
    {
      perror("baseport socket");
      exit(1);
    }
  if ((BPsockfd1 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET ) 
    {
      perror("baseport+1 socket");
      exit(1);
    }  
  setsockopt(BPsockfd,SOL_SOCKET,SO_REUSEADDR,(char *)&dum,sizeof(bool));
  setsockopt(BPsockfd1,SOL_SOCKET,SO_REUSEADDR,(char *)&dum,sizeof(bool));
}

void CServ::Bind()
{ 
  memset(&(my_addr), 0, sizeof(struct sockaddr_in));	/* zero the struct */
  my_addr.sin_family = AF_INET;         /* host byte order */
  my_addr.sin_port = htons(baseport);   /* short, network byte order */
  my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
  if (bind(BPsockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) 
    {
      perror("baseport bind");
      exit(1);
    }

  my_addr.sin_port = htons(baseport1);   /* short, network byte order */
  if (bind(BPsockfd1, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) 
    {
      perror("baseport+1 bind");
      exit(1);
    }
}

void CServ::Listen()
{  
  if (listen(BPsockfd, BACKLOG) == SOCKET_ERROR ) 
    {
      perror("baseport listen");
      exit(1);
    }
  if (listen(BPsockfd1, BACKLOG) == SOCKET_ERROR ) 
    {
      perror("baseport+1 listen");
      exit(1);
    }
  alloc_CClient(BPsockfd);
  ClientsTable[BPsockfd]->state |= T_CONN;
  alloc_CClient(BPsockfd1);
  ClientsTable[BPsockfd1]->state |= T_CONN;
}

void CServ::alloc_CClient(int fd)
{
    ClientsTable[fd] = new CClServ(this, fd);
    fds_used.push_back(fd);
}

void CServ::set_non_blocking ( int m_sock, const bool b )
{

#ifdef __WIN32__ 
// If opts = 0, blocking is enabled 
// If opts != 0, non-blocking mode is enabled.
    int opts;
    if (b)
        opts = 1; 
    else
        opts = 0;
    ioctlsocket(m_sock, FIONBIO, (u_long FAR*) &opts);
#else
    int opts = fcntl ( m_sock, F_GETFL );
    if ( opts < 0 )
        return;
    if (b)
        opts = ( opts | O_NONBLOCK );
    else
        opts = ( opts & ~O_NONBLOCK );  
    fcntl ( m_sock, F_SETFL, opts );
#endif
}


// wait for somebody to connect to us on our port

bool CServ::Connect(int sockfd, int ptype, int port, string str)
{
  socklen_t		sin_size;
  struct sockaddr_in	their_addr;
  int			new_fd;
  string errormsg("accept() on ");

  sin_size = sizeof(their_addr);

  set_non_blocking(sockfd, true);
  if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
    {
      errormsg.append(str);
      perror(errormsg.c_str());
      return false;
    }
  if (DEBUG_L0)
    cout << "Server: got connection on " << port << " from " << inet_ntoa(their_addr.sin_addr) << " on fd " << new_fd << endl;
  alloc_CClient(new_fd);
  ClientsTable[new_fd]->IPAdress.assign(inet_ntoa(their_addr.sin_addr));
  ClientsTable[new_fd]->PortType = ptype;
  ClientsTable[new_fd]->User->fd = new_fd;
  ClientsTable[new_fd]->dest_addr.sin_family = AF_INET;
  ClientsTable[new_fd]->dest_addr.sin_port = htons(port);
  ClientsTable[new_fd]->dest_addr.sin_addr = their_addr.sin_addr;
  memset(& ClientsTable[new_fd]->dest_addr.sin_zero, 0, 8);
  return true;
}

void CServ::KillClient(int fd)
{
  if (DEBUG_L0)
    cout << "Killing Client " << fd << endl; 
  closesocket(fd);
  delete ClientsTable[fd];
  ClientsTable.erase(fd);
  for (list<CUser>::iterator it2 = AccountsLst.begin();
       it2 != AccountsLst.end();
       it2++)
    {
      if (it2->fd == fd)
      {
	if (DEBUG_L1)
	  cout << "re-init User " << it2->login << " connected on fd " << fd << endl;
	it2->connected = false;
	it2->fd = -1;
	it2->name = it2->confname;
	it2->iconId = 0;
	it2->userId = 0;
	it2->version = 0;
	break;
      }
    }
  for (vector<int>::iterator it = fds_used.begin(); it != fds_used.end(); it++)
    {
      if (*it == fd)
	{
	  if (DEBUG_L1)
	    cout << "removing fd " << *it << " from fds_used vector" << endl;
	  fds_used.erase(it);
	  break;
	}    
    }
}

// shut down the socket
void CServ::closesocket(int fd)
{
  if (DEBUG_L1)
    cout << "closing fd " << fd << "..." << endl;
  if (shutdown(fd, SHUT_RDWR) == -1)
    perror("shutdown()");
#ifndef __WIN32__
  if (close(fd) == -1)
    perror("close()");
#endif
}

void CServ::ClientsInitFDS()
{
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    
    for (vector<int>::iterator it = fds_used.begin(); it != fds_used.end(); it++)
    {
      if (ClientsTable[*it]->state & T_READ)
      {
	if (DEBUG_L1)
	  cout << "[waiting read on " << *it << "]" << endl;
    	FD_SET(*it, &readfds);
      }
      if (ClientsTable[*it]->state & T_WRITE)
      {
	if (DEBUG_L1)
	  cout << "[waiting write on " << *it << "]" << endl;
    	FD_SET(*it, &writefds);
      }
    }
}

void CServ::Session()
{
  if (DEBUG_L1)
    cout << endl << "  [New Session]" << endl;
  vector<int> tmpvect;
  tmpvect = fds_used;
  for (vector<int>::iterator it = tmpvect.begin(); it != tmpvect.end(); it++)
    {
      if (FD_ISSET(*it, &readfds))
        {
	  if (ClientsTable[*it]->state & T_CONN)
            {
	      if (!Connect(BPsockfd, T_BP, baseport, "base port")) 
                {
		  if (!Connect(BPsockfd1, T_BP1, baseport1, "dwl/upl port"))
                    cerr << "Connexion port error" << endl;
                }        
            }
	  else
	    ClientsTable[*it]->session(SESSION_IN);
	}    
      else
        {
	  if (FD_ISSET(*it, &writefds))
	    ClientsTable[*it]->session(SESSION_OUT);
	}        
    }
} 

int CServ::First_free_fd()
{
  return 1;
}

int CServ::Max_fd_used()
{
    int maxfd = 0;
    for (vector<int>::iterator it = fds_used.begin(); it != fds_used.end(); it++)
    {
        if (*it > maxfd) 
	  maxfd = *it;
    }
    if (DEBUG_L1)
      cout << "max fd used: " << maxfd << endl;
    return maxfd;
}    

// Serveur main loop
void CServ::LoopZ()
{
  struct timeval tv;

  ServConf.IPaddr.assign(inet_ntoa(my_addr.sin_addr));
  if (DEBUG_L0)
    {
      cout <<"-----------------------------------------------------------------------" << endl;
      cout << "Server: " << ServConf.ServerName << " on port " << ServConf.Port << endl;
      cout << "IP: " << ServConf.IPaddr << endl;
      cout << "agreement file: " << ServConf.Agreement << endl;
      cout << "files folder: " << ServConf.FilesFolder << endl;
      cout << "files type file: " << ServConf.FilesType << endl;
      cout << "Videolan path: " << ServConf.Videolan << endl;
      cout <<"-----------------------------------------------------------------------" << endl;
    }
  tv.tv_sec = TIMEOUT;
  tv.tv_usec = 0;
  
  while (1)
    {
      int nfds, max_fd_used;      
      ClientsInitFDS();
#ifndef __WIN32__
      max_fd_used = Max_fd_used();
      if (DEBUG_L1)
	cout << "[waiting session on " << max_fd_used+1 << "]" << endl;
#endif
      nfds = select(max_fd_used+1, &readfds, &writefds, NULL, 0/*&tv*/);
      if (nfds > 0)
	Session();
      if (nfds == -1 && errno == EINTR)
	continue;
      if (nfds < 0)
	{      
          perror("select()");
          exit (1);
	}
    }
}


