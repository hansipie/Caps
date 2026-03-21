#include "../../../includes/redcap.h"

t_data	*pars_server_name(char *buffer, int len)
{
  char		**tab;
  int		i;
  t_data	*data;

  if(gl_redcap->debug->functions)
    printf("func:pars_server_name\n");
  data = malloc((sizeof(t_data)) * MALLOC);
  i = 0;
  tab = str_to_tab(buffer, len, ' ');
  while(tab[i])
    i++;
  if (i != 1)
    return(NULL);
  if (strncmp(tab[0], "server_name", 11))
    return(NULL);
  return(data);
}

void    server_name(int command)
{
  t_data        *pars;
  int           flag;

  if(gl_redcap->debug->functions)
    printf("func:server_name\n");
  flag = 0;
  if((pars = pars_server_name(gl_redcap->bridge->data->buffer, gl_redcap->bridge->data->len)) != NULL)
    {
      Writen(gl_redcap->bridge->fd, "name of ");
      Writen(gl_redcap->bridge->fd, gl_redcap->engine->server->host);
      Writen(gl_redcap->bridge->fd, " is \"");
      Writen(gl_redcap->bridge->fd, gl_redcap->engine->server->server_name);
      Writen(gl_redcap->bridge->fd, "\"\n");
    }
  else
    Writen(gl_redcap->bridge->fd, "server_name\n");
}

