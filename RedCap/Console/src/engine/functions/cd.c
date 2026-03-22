#include "../../../includes/redcap.h"

int	cd_under(int len, int count)
{
  int	i;

  i = len;
  i--;
  if (i > 1)
    {
      gl_redcap->engine->server->path[i] = 0;
      while((gl_redcap->engine->server->path[i] != '/') && (i > 0))
	{
	  gl_redcap->engine->server->path[i] = 0;
	  i--;
	  count++;
	}
      gl_redcap->engine->server->path[i] = 0;  
    }
  return(count);
}

void	cd_then(char *ret)
{
  if(gl_redcap->debug->functions)
    printf("function:cd_then\n");
  printf("path avant:%s\nret:%s\n", gl_redcap->engine->server->path, ret);
  gl_redcap->engine->server->path = my_strcat(gl_redcap->engine->server->path, ret);
  printf("path entre:%s\n", gl_redcap->engine->server->path);
  gl_redcap->engine->server->path = my_strcat(gl_redcap->engine->server->path, "/");  
  printf("path apres:%s\n", gl_redcap->engine->server->path);
}

void	cd(int command)
{
  char	*buff;
  int	len, count;
  t_data	*data, *tmp;

  if(gl_redcap->debug->functions)
    printf("function:cd\n");
  count = 0;
  data = pars_opt(gl_redcap->bridge->data->buffer, 
		  gl_redcap->bridge->data->len, 
		  command);
  tmp = data;
  if (data != NULL)
    {      
      data = data->next;
      len = strlen(gl_redcap->engine->server->path);
      if (!strcmp(data->buffer, ".."))
	count = cd_under(len, count);
      else
	cd_then(data->buffer);
      if (strlen(gl_redcap->engine->server->path) < 1)
	gl_redcap->engine->server->path = my_strcat(gl_redcap->engine->server->path, "/");
      buff = malloc((strlen(gl_redcap->engine->server->path) + 2 + count) * MALLOC);
      sprintf(buff, "%s\n", gl_redcap->engine->server->path);
      Writen(gl_redcap->bridge->fd, buff);
      free(buff);      
    }
  else
    cd_help(gl_redcap->bridge->fd);
  free_opt(tmp);
}
