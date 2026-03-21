
/* Includes */
#include "../../includes/redcap.h"

/* Global variables */
int daemon_proc;                /* set to nonzero by daemon_init() */

/* Static functions */

/* Main function to handle error */
static void err_doit(int errnoflag, int level, const char *fmt, va_list ap)
{
  int errno_save; /* for storing the errno before printing it */
  int n; /* length of the message to print */
  char buf[MAXLINE + 1]; /* buffer for printing */
  
  /* Save the error number */
  errno_save = errno;
  /* write the message into buf */
  vsnprintf(buf, MAXLINE, fmt, ap);
  /* Retrieve the length of buf */
  n = strlen(buf);
  /* There was an error
     Write the message + the error number to buf */
  if (errnoflag)
    {
      snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
    }
  /* Append a new line character to buf */
  strcat(buf, "\n");
  
  /* If the process was launched as a daemon, write the message
     to the console with level LOG_ERR */
  if (daemon_proc)
    {
      syslog(level, "%s", buf);
    }
  /* The process was launched as normal process */
  else
    {
      /* Empty the std out */
      fflush(stdout);
      /* Write the message to stderr */
      fputs(buf, stderr);
      /* Empty stderr */
      fflush(stderr);
    }  
  return;
}

/* /\* Handle the errors when reading a stream *\/ */
/* static ssize_t mg_read(int fd, char *ptr) */
/* { */
/*   static int read_cnt = 0; */
/*   static char *read_ptr; */
/*   static char read_buf[MAXLINE]; */
  
/*   if (read_cnt <= 0) */
/*     { */
/*     again: */
/*       if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) */
/* 	{ */
/* 	  if (errno == EINTR) */
/* 	    { */
/* 	      goto again; */
/* 	    } */
/* 	  return -1; */
/* 	} */
/*       else if (read_cnt == 0) */
/* 	{ */
/* 	  return 0; */
/* 	} */
      
/*       read_ptr = read_buf; */
/*     } */
/*   read_cnt--; */
/*   *ptr = *read_ptr++; */
/*   return 1; */
/* } */

/* Display an help message and quit */
/* void servusage(char *progname) */
/* { */
/*   err_quit("\nusage: %s\n" */
/* 	   "   -h        this help\n\n" */
/* 	   "You can launch the server in the background like this:\n" */
/* 	   "   %s &\n" */
/* 	   "Then you have to type return before launching any client.\n" */
/* 	   "When finishing with the server, you will have to kill the server like this:\n" */
/* 	   "   kill - 9 %d\n\n" */
/* 	   "Or launch the server in the foreground like this:\n" */
/* 	   "   %s -a\n" */
/* 	   "Then open a new terminal to launch client.\n", progname, progname, (int)getpid(), progname); */
/* } */

/* Bind the socket */
void Bind(int fd, const struct sockaddr *sa, my_socklen_t salen)
{
  if (bind(fd, sa, salen) < 0)
    {
      err_sys("bind error");
    }
}

/* Listen for connection on the listening socket */
void Listen(int fd, int backlog)
{
  if (listen(fd, backlog) < 0)
    {
      err_sys("listen error");
    }
}

/* Function to call to catch a SIGCHLD */
Sigfunc *Signal(int signo, Sigfunc *func)
{
  Sigfunc *sigfunc; /* function to call when the signal raises */

  if ( (sigfunc = mg_signal(signo, func)) == SIG_ERR)
    {
      err_sys("signal error");
    }
  
  return sigfunc;
}

/* Fork a child process */
pid_t Fork(void)
{
  pid_t   pid;
  
  if ( (pid = fork()) == -1)
    {
      err_sys("fork error");
    }

  return pid;
}

/* Close the socket */
void Close(int fd)
{
  if (close(fd) == -1)
    {
      err_sys("close error");
    }
}

/* /\* Handle all requests *\/ */
/* int	Read_from(int sockfd, char *line) */
/* { */
/*   int i; /\* counter for reading a word *\/ */
/*   ssize_t n; /\* length of the line *\/ */

/*   i = 1; */
/*   /\* Discard any data if the client has terminated *\/ */
/*   if ( (n = Readline(sockfd, line, MAXLINE)) == 0) */
/*     { */
/*       printf("connection closed by other end\n"); */
/*       return(0);         /\* connection closed by other end *\/ */
/*     } */
/*   printf("Readline: [%s]\n", line);       */
/* /\*       snprintf(line, sizeof(line),"%s\n", data); *\/ */

/* /\*       /\\* Get the length of line *\\/ *\/ */
/* /\*       n = strlen(line); *\/ */
/* /\*       /\\* Write line to socket *\\/ *\/ */
/* /\*       Writen(sockfd, line, n); *\/ */
/*   return(n); */
/* } */

/* Create a socket */
int Socket(int family, int type, int protocol)
{
  int n; /* the socket to return */
  int	dum;

  dum = 1;
  if ( (n = socket(family, type, protocol)) < 0)
    {
      err_sys("socket error");
    }
  setsockopt(n, SOL_SOCKET, SO_REUSEADDR, &dum, sizeof(dum));
  return n;
}

/* Wrapper for fgets */
char * Fgets(char *ptr, int n, FILE *stream)
{
  char *rptr;
  
  if ( (rptr = fgets(ptr, n, stream)) == NULL && ferror(stream))
    {
      err_sys("fgets error");
    }
  
  return rptr;
}

/* /\* Read a string character after character from a stream *\/ */
/* ssize_t mg_readline(int fd, void *vptr, size_t maxlen) */
/* { */
/*   int n, rc; */
/*   char c, *ptr; */
  
/*   ptr = (char *)vptr; */
/*   for (n = 1; n < (int)maxlen; n++) */
/*     { */
/*       if ( (rc = mg_read(fd, &c)) == 1) */
/* 	{ */
/* 	  *ptr++ = c; */
/* 	  if (c == '\n') */
/* 	    { */
/* 	      break;  /\* newline is stored, like fgets() *\/ */
/* 	    } */
/* 	} */
/*       else if (rc == 0) */
/* 	{ */
/* 	  if (n == 1) */
/* 	    { */
/* 	      return 0;      /\* EOF, no data read *\/ */
/* 	    } */
/* 	  else */
/* 	    { */
/* 	    break;          /\* EOF, some data was read *\/ */
/* 	    } */
/* 	} */
/* 	  else */
/* 	    { */
/* 	    return -1;             /\* error, errno set by read() *\/ */
/* 	    } */
/* 	} */
    
/*   *ptr = 0;       /\* null terminate like fgets() *\/ */
/*   return n; /\* return the number of bytes read *\/ */
/* } */

/* /\* Wrapper for mg_readline *\/ */
/* ssize_t Readline(int fd, void *ptr, size_t maxlen) */
/* { */
/*   ssize_t n; */
  
/*   if ( (n = mg_readline(fd, ptr, maxlen)) < 0) */
/*     err_sys("mg_readline error"); */
/*   return n; */
/* } */

/* Wrapper for fputs */
void Fputs(const char *ptr, FILE *stream)
{
  if (fputs(ptr, stream) == EOF)
    err_sys("fputs error");
}

/* /\* Write a string character after character to a stream *\/ */
/* ssize_t writen(int fd, const void *vptr, size_t n) */
/* { */
/*   size_t nleft; */
/*   ssize_t nwritten; */
/*   const char *ptr; */
  
/*   ptr = (char *)vptr; */
/*   nleft = n; */
/*   while (nleft > 0) */
/*     { */
/*       if ( (nwritten = write(fd, ptr, nleft)) <= 0) */
/* 	{ */
/* 	if (errno == EINTR) */
/* 	  nwritten = 0; /\* and call write() again *\/ */
/* 	else */
/* 	  return -1; /\* error *\/ */
/* 	} */
/*       nleft -= nwritten; */
/*       ptr   += nwritten; */
/*     } */
/*   return n; /\* the number of bytes written *\/ */
/* } */

/* /\* Wrapper for writen *\/ */
/* /\* void Writen(int fd, void *ptr, size_t nbytes) *\/ */
/* void Writen(int fd, void *ptr) */
/* { */
/*   size_t	nbytes;   */

/*   if (gl_redcap->debug->functions) */
/*     printf("functions:Writen\n"); */
/*   nbytes = strlen(ptr); */
/*   if (writen(fd, ptr, nbytes) != (ssize_t)nbytes) */
/*     err_sys("writen error"); */
/* } */

/* Wait for read and write descriptors ready */
int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
  int n;
  
  if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
    err_sys("select error");
  return n;              /* can return 0 on timeout */
}

 /* Initiate a normal termination sequence */
void Shutdown(int fd, int how)
{
  /* Make a half-close on write end */
  if (shutdown(fd, how) < 0)
    err_sys("shutdown error");
 }

/* Action to take when child terminates */
void sig_chld(int signo)
 {
   pid_t pid;
   int stat;
   
   /* Wait for the child to terminate */
   while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
     printf("child %d terminated\n", pid);
   return;
 }

/* Handle signals */
Sigfunc * mg_signal(int signo, Sigfunc *func)
{
  struct sigaction act, oact;
  
  /* Set handler to func */
  act.sa_handler = func;
  /* No other signal is blocked */
  sigemptyset(&act.sa_mask);
  /* Clean up options */
  act.sa_flags = 0;
  /* If signal if SIGALARM, allow the kernel to restart it */
  if (signo == SIGALRM)
    act.sa_flags |= SA_RESTART;
  /* Call sigaction */
  if (sigaction(signo, &act, &oact) < 0)
    return SIG_ERR;
  return oact.sa_handler;
}

/* Handle errors generated by the system */
void err_sys(const char *fmt, ...)
{
  /* Use the variable-length argument list facility */
  va_list         ap;
  va_start(ap, fmt);
  /* Write an error message to stderr or to console and quit */
  err_doit(1, LOG_ERR, fmt, ap);
  va_end(ap);
  exit(EXIT_FAILURE);
}

/* Handle errors not generated by the system,
   i.e. there was no errno generated by the caller */
void err_quit(const char *fmt, ...)
{
  /* Use the variable-length argument list facility */
  va_list         ap;
  va_start(ap, fmt);
  /* Write an error message to stderr or to console and quit */
  err_doit(0, LOG_ERR, fmt, ap);
  va_end(ap);  
  exit(EXIT_FAILURE);
}
