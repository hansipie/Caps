#include "../../includes/redcap.h"

typedef struct          s_thread_bridge
{
  int			fd;
  int			alive;
  pthread_t		*thread;
}			t_thread_bridge;

void	show_bridge()
{
      printf("bridge:\n\tport %d\n\tpasswd %p\n\tfd %d\n\tnbr %d\n\tdata %p\n",
	     gl_redcap->bridge->port, gl_redcap->bridge->passwd, 
	     gl_redcap->bridge->fd, gl_redcap->bridge->nbr, 
	     gl_redcap->bridge->data);  
}

void	boucle_bridge(t_thread_bridge *t_bridge)
{
  char	*func_return;
  char	*tmp;
  
  if (gl_redcap->debug->functions)
    printf("function:boucle_bridge\n");
  for ( ; ; )
    {
      if (t_bridge->alive == 0)
	break;
      gl_redcap->bridge->fd = t_bridge->fd;
      gl_redcap->bridge->data = Read_from_bridge(t_bridge->fd, gl_redcap->bridge->data);
      if (gl_redcap->bridge->data)
	{
	  if ((gl_redcap->bridge->data->len == -1))
	    {
	      t_bridge->alive = 0;
	      break;
	    }
	  if(gl_redcap->bridge->data->len > 0)
	    {
	      tmp = malloc((gl_redcap->bridge->data->len) * MALLOC);
	      bcopy(gl_redcap->bridge->data->buffer, tmp, gl_redcap->bridge->data->len);
	      func_return = func_commands(t_bridge->fd);
	      if (!strcmp("", func_return)
		  && strcmp(gl_redcap->bridge->data->buffer, func_return))
		completion(t_bridge->fd);
	      free(tmp);
	    }	  
	}
      if (gl_redcap->bridge->data)
	gl_redcap->bridge->data = remove_data(gl_redcap->bridge->data);
      if (t_bridge->alive == 1)
	prompt(t_bridge->fd);
    }
  gl_redcap->bridge->data = NULL;
  if(gl_redcap->debug)
    printf("exit thread of IHM on fd:%d\n", t_bridge->fd);
  close(t_bridge->fd);
  gl_redcap->bridge->nbr--;
}

void	launch_bridge(t_thread_bridge *t_bridge)
{
  if (gl_redcap->debug->functions)
    printf("function:launch_bridge\n");
  Writen(t_bridge->fd, "\n : Welcome to RedCap Console Client :\n");
  me_info(t_bridge->fd);
  prompt(t_bridge->fd);
  boucle_bridge(t_bridge);
  close(t_bridge->fd);
  pthread_detach(*t_bridge->thread);
  free(t_bridge->thread);
  free(t_bridge);
}

void		main_bridge()
{
  int			fd;
  int			connfd;
  struct sockaddr_in	servaddr;
  struct sockaddr_in	cliaddr;
  my_socklen_t		clilen;
  pthread_attr_t	attr;
  t_thread_bridge	*t_bridge;
  
  if (gl_redcap->debug->functions)
    printf("function:main_bridge\n");
  t_bridge = malloc(sizeof(t_thread_bridge) * MALLOC);
  t_bridge->alive = 0;
  t_bridge->fd = 0;
  t_bridge->thread = NULL;
  gl_redcap->bridge->nbr = 0;
  fd = Socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family      = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port        = htons(gl_redcap->bridge->port);
  Bind(fd, (SA *) &servaddr, sizeof(servaddr));
  Listen(fd, LISTENQ);
  for ( ; ; )
    {
      clilen = sizeof(cliaddr);
      if ((connfd = accept(fd, (SA *) &cliaddr, &clilen)) < 0)
	{
	  if (errno == EINTR)
	    continue;
	  else
	    err_sys("bridge accept error");
	}
      else
	{
	  gl_redcap->bridge->nbr++;
	  if (gl_redcap->bridge->nbr > 1)
	    {
	      Writen(t_bridge->fd, "\nAn other IHM was OPEN\nConnection will be closed\n");
	      t_bridge->alive = 0;	      
	    }
	  t_bridge = malloc(sizeof(t_thread_bridge) * MALLOC);
	  t_bridge->fd = connfd;
	  pthread_attr_init(&attr);
	  t_bridge->thread = malloc(sizeof(pthread_t) * MALLOC);
	  t_bridge->alive = 1;
	  pthread_create(t_bridge->thread, NULL, (void *)launch_bridge, (void *)t_bridge);
	  if (gl_redcap->debug->thread)
	    perror("pthread_create in main_bridge");
	}
    }
}
