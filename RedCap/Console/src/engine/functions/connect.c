#include "../../../includes/redcap.h"

int	boucle_client(t_server *server)
{
  char *buff;
  int		err;
  
  if (gl_redcap->debug->functions)
    printf("function:boucle_client\n");
  for ( ; ; )
    {
      pthread_mutex_lock(&gl_redcap->engine->server->mutex);
      if (server->connected == 4)
	{
	  if ((err =pthread_mutex_unlock(&gl_redcap->engine->server->mutex)) != 0)
	    printf("lock in boucle_client error of gl_redcap->engine->server->mutex\n");
	  else
	    printf("lock in boucle_client succed of gl_redcap->engine->server->mutex\n");
	  return (-3);
	}
      if (server->connected >= 2)
	{
	  server->data = Read_from_server(server->fd, server->data, server);
	  printf("receive from server:%s buff of size:%d\n", server->name, server->data->len);
	  if (server->connected == 4)
	    {
	      close(server->fd);
	      server->fd = -42;
	      buff = malloc((29 + strlen(server->host)) * MALLOC);
	      sprintf(buff, "\ndisconnected from %s\n", server->host);
	      Writen(gl_redcap->bridge->fd, buff);
	      free(buff);
	      //	      manage_server(server->thread, server, "extract_server");
	      prompt(gl_redcap->bridge->fd);	      
	      if ((err = pthread_mutex_unlock(&gl_redcap->engine->server->mutex)) != 0)
	      perror("pthread_mutex_unlock in boucle_client of gl_redcap->engine->server->mutex");
	      return (-3);
	    }
	  if (server->data->len == -1)
 	    {
	      server->connected = 0;
	      close(server->fd);
	      server->fd = -42;
	      buff = malloc((29 + strlen(server->host)) * MALLOC);
	      sprintf(buff, "\ndisconnected from %s\n", server->host);
	      printf("c celui la#######################\n");
	      Writen(gl_redcap->bridge->fd, buff);
	      free(buff);
	      prompt(gl_redcap->bridge->fd);
	      if (gl_redcap->engine->server->auto_reconnect == 0)
		{
		  server->connected = 2;
		  return(0);
		}
	      pthread_mutex_unlock(&gl_redcap->engine->server->mutex);
	      perror("pthread_mutex_unlock 1 in boucle_client of gl_redcap->engine->server->mutex");
	      return(-1);
	    }
	  else
	    {
	      pthread_cond_broadcast(&gl_redcap->engine->cond);
	      perror("pthread_cond_broadcast in boucle_client of gl_redcap->engine->cond: ");
	    } 
	  if (server->connected < 2)
	    {
	      printf("plus connecte\n");
	      pthread_mutex_unlock(&gl_redcap->engine->server->mutex);
	      perror("pthread_mutex_unlock 2 in boucle_client of gl_redcap->engine->server->mutex: ");
	      return(-2);
	    }
	}
      pthread_mutex_unlock(&gl_redcap->engine->server->mutex);
    }
  printf("server %s close connection\n", server->host);
  return(-1);
}
  
int	init_socket(t_server *server)
{
  struct sockaddr_in servaddr;
  
/*   printf("server:%s, port:%d, connected:%d\n", server->host, server->port, server->connected); */
  if (gl_redcap->debug->functions)        
    printf("function:init_socket\n");
  server->fd = Socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(server->port);
  inet_pton(AF_INET, server->host, &servaddr.sin_addr);
  if (connect(server->fd, (SA *) &servaddr, sizeof(servaddr)) < 0)
    {
      perror("connect");
      if(gl_redcap->debug->other)
	printf("Server return an error.\n");
      server->connected = 0;
      Writen(gl_redcap->bridge->fd, "\nServer ");
      Writen(gl_redcap->bridge->fd, server->host);
      Writen(gl_redcap->bridge->fd, " return an error.\n");
      prompt(gl_redcap->bridge->fd);
      return(-1);
    }
  server->connected = 1;
  if (gl_redcap->debug->other)        
    printf("connected but not logged\n");
  Writen(gl_redcap->bridge->fd, "\n");
  gl_redcap->engine->server = server;
  prompt(gl_redcap->bridge->fd);
  Writen(gl_redcap->bridge->fd, "\nconnected to ");
  Writen(gl_redcap->bridge->fd, server->host);
  if (server->caps_flag)
    Writen(gl_redcap->bridge->fd, " with CAPS protocol\n");
  else
    Writen(gl_redcap->bridge->fd, " without CAPS protocol\n"); 
  return(1);
}


void	TRTP(t_server *server)
{
  char	str[8];
  int	i;
  
  if (gl_redcap->debug->functions)        
    printf("function:TRTP\n");
  if (server->caps_flag == 1)
    write(server->fd, "TRTPCAPS\0\1\0\0", 12);
  else
    write(server->fd, "TRTPHOTL\0\1\0\2", 12);
  read(server->fd, str, 8);
  if(strncmp("TRTP", str, 4) == 0)
    {
      i = 4;
      while (i <= 7)
	{
	  if (str[i] != 0)
	    {
	      server->connected = 1;
	      break;
	    }
	  else
	    i++;
	}
      if (i == 8)
	server->connected = 2;
      else
	{	  
	  server->connected = -1;
	  close(server->fd);
	  server->fd = -42;
	}
    }
}

void	reconnect(t_server *server, int time)
{
  char	*str, *buff;
	
  str = malloc((5) * MALLOC);
  sprintf(str, "%d", time);
  if (gl_redcap->debug->functions)
    printf("function:reconnect\n");
  if (server->fd != -42)
    close(server->fd);
  server->fd = -42;
  buff = malloc((strlen("\nrelogin in ") + strlen(str) + strlen("s on ") + strlen(server->host) + 2) * MALLOC);
  sprintf(buff, "\nrelogin in %ss on %s\n", str, server->host);
  Writen(gl_redcap->bridge->fd, buff);
  prompt(gl_redcap->bridge->fd);
  server->data = NULL;
  free(str);
  free(buff);
  sleep(time);  
}

void	launch_connect(t_server *server)
{
  int	   ret;
  int	   time;
  
  time = 0;
  if (gl_redcap->debug->functions)      
    printf("function:launch_connect\n");
  if (server->prev == NULL && server->next == NULL)
    manage_server(server->thread, server, "insert_server");
  Writen(gl_redcap->bridge->fd, "\n");
  prompt(gl_redcap->bridge->fd);
  while ((ret = init_socket(server)) == -1)
    {
      time += 5;
      if (time >= 60)
	time = 60;
      printf("reconnect car init socket\n");
      reconnect(server, time);
    }
  if (ret == 1)
    {
      TRTP(server);
      if(server->connected == -1)
	{
	  server->connected = 1;
	  server->caps_flag = 0;
	  Writen(gl_redcap->bridge->fd, "disconnecting because server don't support CAPS protocol");
	  reconnect(server, 0);
	  launch_connect(server);
	  return;
	}
      else
	{
	  pthread_mutex_init(&server->mutex, NULL);
	  gl_redcap->engine->server = server;
	  main_login(server);
	  ret = boucle_client(server);
	  if ((ret == -3) && (ret == 0))
	    {
	      gl_redcap->engine->server->connected = 2;
	      return;
	    }
	  if (ret == -1)
	    {
	      reconnect(server, 5);
	      launch_connect(server);
	      return;
	    }
	}
    }
}

t_server	*pars_opt_connect(char *buffer, int len)
{
  char	**tab;
  char	**tab2;
  char	**tab3;
  t_server	*server;
  int		nbr;
  int		i;
  
  tab = str_to_tab(buffer, len, ' ');
  if (strcmp(tab[0], "connect"))
    {
      str_to_tab_clean(tab);
      return(NULL);
    }
  tab = str_to_tab(buffer, len, ' ');
  if (strcmp(tab[0], "connect"))
    {
      str_to_tab_clean(tab);
      return(NULL);
    }
  if (!strcmp(tab[0], "connect") && !tab[1])
    {
      str_to_tab_clean(tab);
      if (gl_redcap->engine->server->host && !strcmp(gl_redcap->engine->server->host, "Client RedCap"))
	return (NULL);
      return(gl_redcap->engine->server);
    }
  server = malloc((sizeof(t_server)) * MALLOC);
  server->id = 0;
  server->connected = 0;
  server->caps_flag = 1;
  server->auto_agree = 0;
  server->auto_reconnect = 1;
  server->version=0;
  server->server_name = strdup("no_name");
  server->host = NULL;
  server->transfert = NULL;
  server->port = 5500;
  server->login = NULL;
  server->passwd = NULL;
  server->fd = -42;
  server->path = strdup("/");
  server->data = NULL;
  server->name = NULL;
  server->icon = 0;
  server->prev = NULL;
  server->next = NULL;
  
  for (nbr = 0; tab[nbr]; nbr++)
    ;
  if (nbr == 3)
    {
      if(!strncmp(tab[2], "auto_agree", 10))
	server->auto_agree = 1;
      nbr--;
    }
  if (nbr == 1)
    return(NULL);
  
  tab2 = str_to_tab(tab[1], strlen(tab[1]), '@');
  
  str_to_tab_clean(tab);
  
  nbr = 0;
  if (tab2)
    for (; tab2[nbr]; nbr++)
      ;
  
  if (nbr != 2 || strlen(tab2[0]) == 0 || strlen(tab2[1]) == 0)
    return(NULL);
  
  for (i = 0; i <= 1; i++)
    {
      tab3 = str_to_tab(tab2[i], strlen(tab2[i]), ':');
      for (nbr = 0; tab3[nbr]; nbr++)
	;
      if (nbr > 2)
	return(NULL);
      if(i == 0)
	{
	  server->login = strdup(tab3[0]);
	  if (nbr == 2)
	    server->passwd = strdup(tab3[1]);
	  else
	    server->passwd = NULL;
	}
      else
	{
	  server->host = strdup(tab3[0]);
	  if (nbr == 2)
	    server->port = atoi(strdup(tab3[1]));
	}
      str_to_tab_clean(tab3);
    }
  str_to_tab_clean(tab2);
  return(server);
}	

void    main_connect(int connfd)
{
  pthread_attr_t        attr;
  pthread_t             *thread;
  t_server		*server;
  
  if(gl_redcap->debug->functions)
    printf("func:main_connect\n");
  if((server = pars_opt_connect(gl_redcap->bridge->data->buffer, 
			gl_redcap->bridge->data->len)) != NULL)
    {
      Writen(gl_redcap->bridge->fd, "connecting to:");
      Writen(gl_redcap->bridge->fd, server->host);
      Writen(gl_redcap->bridge->fd, "\n");
      thread = malloc((sizeof(pthread_t)) * MALLOC);
      pthread_attr_init(&attr);
      server->thread = thread;
      if(pthread_create(thread, NULL, (void *)launch_connect, (void *)server) != 0)
	{
	  printf("pthread_create error in open_connect in connect.c\n");
	  return;
	}
    }
  else
    help_connect(gl_redcap->bridge->fd);
}
