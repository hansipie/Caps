#include "../../../includes/redcap.h"

t_manage_server		gl_manage_server[] =
  {
    {"goto_server",	goto_server},
    {"list_server",	list_server},
    {"extract_server",	extract_server},
    {"remove_server",	remove_server},
    {"insert_server",	insert_server},
    {0, 0},
  };

void	next_server()
{
  gl_redcap->engine->server = gl_redcap->engine->server->next;
  if (gl_redcap->engine->server->id != 0)
    { 
      if ((!strncmp(gl_redcap->name, gl_redcap->engine->server->name, 
		    strlen(gl_redcap->name)))
	  || (gl_redcap->engine->server->icon != gl_redcap->icon))
	{
	  gl_redcap->engine->server->name = strdup(gl_redcap->name);
	  gl_redcap->engine->server->icon = gl_redcap->icon;
	  send_cli_nfo(SET_CL_USR_INF, gl_redcap->engine->server->fd);
	}
    }
}

void    previous_server()
{
  gl_redcap->engine->server = gl_redcap->engine->server->prev;
  if (gl_redcap->engine->server->id != 0)
    { 
      if ((!strcmp(gl_redcap->name, gl_redcap->engine->server->name))
	  || (gl_redcap->engine->server->icon != gl_redcap->icon))
	{
	  gl_redcap->engine->server->name = strdup(gl_redcap->name);
	  gl_redcap->engine->server->icon = gl_redcap->icon;
	  send_cli_nfo(SET_CL_USR_INF, gl_redcap->engine->server->fd);
	}
    }
}

t_server	*goto_server(pthread_t *thread, t_server *server)
{
  int	   flag;

  flag = 0;
  while(strcmp(gl_redcap->engine->server->host, gl_redcap->bridge->data->buffer))
    {
      if (strcmp(gl_redcap->engine->server->host, "Client RedCap"))
	flag++;
      if (flag > 1)
	return(NULL);
      gl_redcap->engine->server = gl_redcap->engine->server->next;
    }
  return(NULL);
}

t_server	 *list_server(pthread_t *thread, t_server *server)
{
  t_server *server_c;
  int connfd;
  char	*str;
  int	i;

  if (gl_redcap->debug->functions)
    printf("function:list_server\n");
  Writen(gl_redcap->bridge->fd, "server_list:\n");
  i = 1;
  connfd = gl_redcap->bridge->fd;
  server_c = gl_redcap->engine->server;
  while(strncmp(gl_redcap->engine->server->host, "Client RedCap", 13))    
    gl_redcap->engine->server = gl_redcap->engine->server->next;
  gl_redcap->engine->server = gl_redcap->engine->server->next;
  str = malloc((5) * MALLOC);
  while(strncmp(gl_redcap->engine->server->host, "Client RedCap", 13))
    {
      sprintf(str, "%d", gl_redcap->engine->server->id);
      Writen(connfd, str);
      Writen(connfd, ":\t");
      if(strncmp(gl_redcap->engine->server->server_name, "no_name", 7))
	{
	  Writen(connfd, "\"");
	  Writen(connfd, gl_redcap->engine->server->server_name);
	  Writen(connfd, "\" \t");
	}
      else
	Writen(connfd, "\"no name\"\t");
      Writen(connfd, gl_redcap->engine->server->login);
      Writen(connfd, "@");
      Writen(connfd, gl_redcap->engine->server->host);
      Writen(connfd, ":");
      sprintf(str, "%d", gl_redcap->engine->server->port);
      Writen(connfd, str);
      Writen(connfd, "\t Status ");
      if (gl_redcap->engine->server->caps_flag == 1)
	Writen(connfd, "\tCaps:1\t");
      else
	Writen(connfd, "\tCaps:0\t");
      if (gl_redcap->engine->server->connected == 0)
	Writen(connfd, "No host\n");
      if (gl_redcap->engine->server->connected == 1)
	Writen(connfd, "No Hotline server\n");
      if (gl_redcap->engine->server->connected == 2)
	Writen(connfd, "Not logged in\n");
      if (gl_redcap->engine->server->connected == 3)
	Writen(connfd, "Logged\n");
      if (gl_redcap->engine->server->connected == 4)
	Writen(connfd, "disagree\n");
      i++;
      gl_redcap->engine->server = gl_redcap->engine->server->next;
    }
  free(str);
  gl_redcap->engine->server = server_c;
  return(NULL);
}

void		lst_server()
{
  pthread_t *thread = NULL;
  t_server *server = NULL;

  list_server(thread, server);
}

t_server *extract_server(pthread_t *thread, t_server *server)
{
  t_server	*next;
  t_server	*prev;

  if (gl_redcap->debug->functions)
    printf("***function:extract_server\n");
  next = server->next;
  prev = server->prev;
  prev->next = next;
  next->prev = prev;
  free(server);
  gl_redcap->engine->server = prev;
  return(prev);
}

t_server *remove_server(pthread_t *thread, t_server *server)
{
  t_server *prev;

  if (gl_redcap->debug->functions)
    printf("***function:remove_server\n");
  prev = server->prev;
  prev->next = server->next;
  close(server->fd);
  free(server);
  return(prev);
}

int next_id(int *tab, int len)
{
  int n;
  int t;
  int trouv;

  trouv = 0;
  for (n = 1; n <= len + 1; n++)
    {
      for (t = 0; tab[t]; t++)
	if (tab[t] == n)
	  trouv = 1;
      if (trouv == 1)
	trouv = 0;
      else
	return (n);
    }
  return (len + 1);
}

t_server *insert_server(pthread_t *thread, t_server *new)
{
  int		nbr;
  t_server	*server;
  t_server	*serverredcap;
  t_server	*prev;
  int  *tabid;
  
  if (gl_redcap->debug->functions)
    printf("function:insert_server\n");    
  new->name = strdup("none");
  server = gl_redcap->engine->server;
  while(strncmp(server->host, "Client RedCap", 13))
      server = server->next;
  serverredcap = server;
  server = server->next;
  nbr = 1;
  while(strncmp(server->host, "Client RedCap", 13))
    {
      nbr++;
      server = server->next;
    }
  server = server->next;
  tabid = malloc(((nbr + 1) * (sizeof(int))) * MALLOC);
  tabid[0] = 0;
  nbr = 1;
  while(strncmp(server->host, "Client RedCap", 13))
    {
      tabid[nbr - 1] = server->id;
      tabid[nbr] = 0;
      server = server->next;
      nbr++;
    }
  new->id = next_id(tabid, nbr - 1);
  free(tabid);
  new->thread = thread;
  prev = serverredcap->prev;
  prev->next = new;
  new->prev = prev;
  new->next = serverredcap;
  serverredcap->prev = new;
  gl_redcap->engine->server = new;
  return(new);
}

t_server *manage_server(pthread_t *thread, t_server *server, char *manage)
{
  int	i, err;
  t_server	*ret;

  for (i = 0; gl_manage_server[i].manage; i++)
    {
      if (strcmp(manage, gl_manage_server[i].manage) == 0)
	{
	  pthread_mutex_lock(&gl_redcap->engine->mutex);
	  ret = gl_manage_server[i].manage_fct(thread, server);
	  if ((err = pthread_mutex_unlock(&gl_redcap->engine->mutex)) != 0)
	    printf("unlock in manage_server error of gl_redcap->engine->mutex\n");
	  else
	    printf("unlock in manage_server succed of gl_redcap->engine->mutex\n");
	  break;
	}
    }
  return(ret);	  
}
