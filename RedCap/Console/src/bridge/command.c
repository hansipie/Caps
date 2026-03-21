#include "../../includes/redcap.h"

#define	NOT_CONNECTED	0
#define	CONNECTED	1
#define	RESTRICTED	2

t_function              gl_command[] =
  {
/*     command			function call		argc	space */
    { "get_user",		get_user,		2,	0,	CONNECTED },
    { "rename",			rename_file,   		3,	1,	CONNECTED },
    { "my_access",		my_access,		1,	0,	CONNECTED },
    { "delete_user",		delete_user,		2,	0,	CONNECTED },
    { "new_user",		new_user,		5,	1,	CONNECTED },
    { "set_user_edit",		set_user_edit,		5,	1,	CONNECTED },
    { "file_info",		get_file_info,		2,	1,	CONNECTED },
    { "lrename",			lrename,			3,	1,	NOT_CONNECTED },
    { "connect",		main_connect,		1,	0,	NOT_CONNECTED },
    { "me_info",		me_info,		1,	0,	NOT_CONNECTED },
    { "set_user",		set_user,		1,	0,	NOT_CONNECTED },
    { "next_server",		next_server,		1,	0,	NOT_CONNECTED },
    { "prev_server",		previous_server,	1,	0,	NOT_CONNECTED },
    { "list_server",		lst_server,		1,	0,	NOT_CONNECTED },
    { "bookmark",		get_bookmark,		1,	0,	NOT_CONNECTED },
    { "lls",			main_file_list_client,	1,	0,	NOT_CONNECTED },
/*     { "?",			help,			1,	0,	NOT_CONNECTED }, */
/*     { "help",			help,			1,	0,	NOT_CONNECTED }, */
    { "disconnect",		disconnect,		1,	0,	CONNECTED },
    { "get_user_list",		get_user_list,		1,	0,	CONNECTED },
    { "ls",			get_file_list,		1,	0,	CONNECTED },
    { "cd",			cd,			2,	1,	CONNECTED },
    { "pwd",			pwd,			1,	0,	CONNECTED },
    { "lrm",   			lrm,			2,	1,	CONNECTED },
    { "lmkdir",   		lmkdir,			2,	1,	CONNECTED },
    { "mkdir",   		my_mkdir,      		2,	1,	CONNECTED },
    { "rm",   		delete_file,      	2,	1,	CONNECTED },
    { "server_name",   		server_name,	       	1,	0,	CONNECTED },
/*     { "user_info",   		user_info,	       	2,	0,	CONNECTED }, */
    { "agree",			agreed,			1,	0,	CONNECTED },
    { "disagree",		disagreed,		1,	0,	CONNECTED },
    { "preview",		preview,		2,	1,	CONNECTED },
    { "download",		download,		2,	1,	RESTRICTED },
    { "upload",			upload,			2,	1,	RESTRICTED },
    { "task",			task,			1,	0,	RESTRICTED },
    { "send_broadcast",		main_send_broadcast,	2,	1,	RESTRICTED },
    { "send_chat",		main_send_chat,		2,	1,	RESTRICTED },
    { "send_priv_chat",		main_send_priv_chat,	3,	1,	RESTRICTED },
    { "stop_task",		stop_task,		2,	0,	RESTRICTED },
    { "delete_task",		delete_task,		2,	0,	RESTRICTED },
    { "cancel_task",		cancel_task,		2,	0,	RESTRICTED },
    { "get_login_list",		get_login_list,		1,	0,	RESTRICTED },
    {0, 0, 0, 0, 0},
/*     { "new_connect", new_connect }, */
/*     { "next_connect", next_connect }, */
/*     { "list_connect", list_connect }, */
/*     { "goto_server", goto_server}, */
/*     { "reconnect", re_login }, */
/*     { "quit", logout_exit }, */
  };

void	help(int fd)
{
  int	i;

  for (i = 0; gl_command[i].function; i++)
    {
      if (gl_command[i].status > NOT_CONNECTED)
	{
/* 	  Writen(fd, gl_command[i].function); */
/*           Writen(fd, "\n"); */
	}
      else
	{
	  Writen(fd, gl_command[i].function);
	  Writen(fd, "\n");
	}
    }
}

void	completion(int fd)
{
  char **tab;
  char	*str, *str2;
  int	i;
  int	flag;
  
  if (gl_redcap->debug->functions == 1)
    printf("function:completion\n");
  flag = 0;
  printf("trying to complete [%s] - (%d)\n", 
	 gl_redcap->bridge->data->buffer, 
	 gl_redcap->bridge->data->len);
  tab = str_to_tab(gl_redcap->bridge->data->buffer, gl_redcap->bridge->data->len, ' ');
  str = strdup(tab[0]);
  printf("str avant:%s\n", str);
  for (i = 0; gl_command[i].function; i++)
    {
      printf("boucle:%d gl_command:[%s] str:[%s]\n", i, gl_command[i].function, str);
      if (!strncmp(gl_command[i].function, str, strlen(str)))
	{
	  str2 = malloc(strlen("Completion: ") + strlen(gl_command[i].function) + 2);
	  sprintf(str2, "Completion: %s\n", gl_command[i].function);
/* 	  Writen(fd, "Completion : "); */
/* 	  Writen(fd, gl_command[i].function); */
/* 	  Writen(fd, "\n"); */
	  Writen(fd, str2);
	  free(str2);
	  flag++;
	}
    }
  if (flag == 0)
    Writen(fd, "Command not found. No such Command\n");
  free(str);
  printf("fin completion\n");
}

int	handle_fromCRLF(char *data, int len)
{
  int i;
  
  if (gl_redcap->debug->functions == 1)
    printf("function:handle_fromCRLF\n");
  i = 0;
  while (data[i])
    {
      if (((data[i] == 13) && (data[i+1] == 10)) || ((data[i] == 10) && (data[i+1] == 13)))
        {
	  data[i] = 0;
	  data[i+1] = 0;
	  len -= 2;
	  break;
        }
      i++;
    }
  return (len);
}

char     *func_commands(int fd)
{
  int   i;
  int	flag;
  char	*command;

  flag = 0;
  gl_redcap->bridge->data->len = handle_fromCRLF(gl_redcap->bridge->data->buffer, gl_redcap->bridge->data->len);
  command = get_command(strdup(gl_redcap->bridge->data->buffer));
  if (command != 0)
    {
      for (i = 0; gl_command[i].function; i++)
        {
          if (strcmp(command, gl_command[i].function) == 0)
            {
	      command +=  gl_redcap->bridge->data->len + 1;
	      pthread_mutex_lock(&gl_redcap->engine->server->mutex);
              gl_command[i].function_fct(i);
	      pthread_mutex_unlock(&gl_redcap->engine->server->mutex);
	      flag = 1;
              break;
            }
        }
      if (flag == 0)
	return("");	
    }
  return(gl_redcap->bridge->data->buffer);
}

char	*get_command(char *str)
{
  char	*s;

  s = str;
  while(*str)
    {
      if(*str == ' ')
	{
	  *str = '\0';
	  return(s);
	}
      str++;
    }
  return(s);
}

t_data	*pars_nospace(char *buff, int len_buff, int command)
{
  char		**tab;
  int		i;
  t_data	*ret, *tmp;  

  if(gl_redcap->debug->functions)
    printf("function:pars_nospace\n");
  tab = str_to_tab(buff, len_buff, ' ');
  for (i = 0; tab[i]; i++)
	;
  if ((i != gl_command[command].argc))
    {
      if(gl_redcap->debug->functions)
	printf("\tpars_opt: args error\n");
      str_to_tab_clean(tab);
      return(NULL);
    }
  i = 0;
  tmp = malloc(sizeof(t_data));
  ret = NULL;
  while (tab[i])
    {
      if (i != 0)
	tmp = malloc(sizeof(t_data));
      tmp->buffer = malloc(strlen(tab[i]) + 2);
      tmp->buffer = my_strndup(tab[i], strlen(tab[i]));
      tmp->len = strlen(tmp->buffer);
      printf("data->buffer:[%s] len:%d\n", tmp->buffer, tmp->len);
      tmp->next = NULL;
      ret = insert_data(tmp, ret);
      i++;
    }
  str_to_tab_clean(tab);
  return(ret);
}
 
t_data	*pars_space(char *buff, int len_buff, int command)
{
  char		**tab;
  int		i, cpt;
  t_data	*ret, *tmp;

  if(gl_redcap->debug->functions)
    printf("function:pars_space\n");
  cpt = 0;
  i = 0;
  while (i < len_buff)
    {
      if (buff[i] == '"')
	cpt++;
      i++;
    }
  tab = str_to_tab(buff, len_buff, '"');
  for (i = 0; tab[i]; i++)
    ;
  printf("cpt:%d i:%d\n", cpt, i);
  if (((gl_command[command].argc == i) && (cpt == (i*2 - 2))) ||
      (!strcmp(gl_command[command].function, "rename") && (cpt == 4) && (i == 4)) || 
      (!strcmp(gl_command[command].function, "lrename") && (cpt == 4) && (i == 4)) ||
      ((!strcmp(gl_command[command].function, "new_user") || 
	!strcmp(gl_command[command].function, "set_user_edit")) && (cpt == 8) && (i == 8)))
    {
      i = 0;
      tmp = malloc(sizeof(t_data));
      ret = NULL;
      while (tab[i])
	{
	  if (i != 0)
	    tmp = malloc(sizeof(t_data));
	  tmp->buffer = malloc(strlen(tab[i]) + 2);
	  tmp->buffer = my_strndup(tab[i], strlen(tab[i]));
	  printf("data->buffer i:%d:[%s]\n", i, tmp->buffer);
	  tmp->next = NULL;
	  ret = insert_data(tmp, ret);
	  i++;
	}
      str_to_tab_clean(tab);
      tmp = ret;
      while (tmp)
	{
	  printf("data->buffer: [%s]\n", tmp->buffer);
	  tmp = tmp->next;
	}
      return(ret);
    }
  if(gl_redcap->debug->functions)
    printf("\tpars_opt: args error\n");
  str_to_tab_clean(tab);
  return(NULL);
}

t_data	*pars_opt(char *buff, int len_buff, int command)
{
  t_data	*ret;

  if(gl_redcap->debug->functions)
    printf("func:pars_opt\n");
  if (!gl_command[command].space)
    ret = pars_nospace(buff, len_buff, command);
  else
    ret = pars_space(buff, len_buff, command);
  return(ret);
}

void	free_opt(t_data *ret)
{  
  if(gl_redcap->debug->functions)
    printf("function:free_opt\n");
  while (ret)
    ret = remove_data(ret);
}
