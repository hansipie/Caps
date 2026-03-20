#include "wrapsock.h"

int main(int argc, char *argv[])
{
  /* Variables */
  int listenfd;   /* passive sockets' file descriptors */
  int connfd;     /* active sockets' file descriptors */
  int chopt;      /* dummy variable for options test */
  int port_served = 0;    /* port to connect */
  pid_t childpid;                 /* pid of the child process */
  my_socklen_t clilen;               /* size of the client's address structure */
  char progname[MAXLINE];         /* program's name */
  struct sockaddr_in cliaddr; /* client address structure */
  struct sockaddr_in servaddr; /* server's address structure */
  
  /* Functions */
  void sig_chld(int signo);       /* function to catch the end of child process */
  
  /* Retrieve the program's name */
  strcpy(progname, argv[0]);
  
  /* Check for a valid option */
  while ((chopt = getopt(argc, argv, "h")) != -1)
    {
      switch (chopt)
	{
	case 'h':
	  {
	    /* Display help and quit */
	    servusage(progname);
	  }
	}
    }
  
  /* Compute the real port number */
  port_served = SERV_PORT1;
  
  /* Help message for the user */
  fprintf(stdout, "\nThe server %s will be attached to port %d.\n"
	  "Its pid is %d.\n\nIf you have launched the server as a "
	  "background process,\nreturn to continue and launch clients."
	  "\nYou will have to kill the server with:\n   kill -9 %d\n"
	  "when finishing with it.\n\nIf you have launched the server"
	  "as a foreground process,\nopen a new terminal to launch a "
	  "client.\nYou will have to type:\n   ctrl-C\nto quit the "
	  "server when finishing with it."
	  "\n\n", progname, port_served, (int)getpid(), (int)getpid());
  
  /* Create the socket */
  listenfd = Socket(AF_INET, SOCK_STREAM, 0);
  
  /* Fill in the address structure */
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family      = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port        = htons(port_served);
  
  /* Attach the socket to the address */
  Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
  
  /* Convert the active socket into passive socket */
  Listen(listenfd, LISTENQ);
  
  /* Set the signal to catch the end of child's process */
  Signal(SIGCHLD, sig_chld);
  
  for ( ; ; )
    {
      clilen = sizeof(cliaddr);       /* retrieve the size of address structure */
      
      /* Duplicates the listening socket */
      if ( (connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0)
	{
	  /* Accept was interrupted by system call - so restart it manually if needed */
	  if (errno == EINTR)
	    {
	      continue;
	    }
	  /* Any other error */
	  else
	    {
	      err_sys("accept error");
	    }
	}
      
      /* Fork a process for answering each client's request */
      if ( (childpid = Fork()) == 0)
	{
	  /* Process Pi */
	  
	  /* Close the listening socket on child side */
	  Close(listenfd);
	  
	  /* Perform the requested job */
	  str_serv1(connfd/* , data */);
	  
	  exit(EXIT_SUCCESS);
	}
      
      /* Parent closes connected socket */
      /* End of child is handled by Signal function */
      Close(connfd);
    }
  
  /* Never goes here, just to be consistent with main's declaration */
  return 0;
}
