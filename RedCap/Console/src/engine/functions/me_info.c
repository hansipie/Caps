#include "../../../includes/redcap.h"

void	set_user(int command)
{
  char  **tab;
  int	nbr;

  nbr = 0;
  tab = str_to_tab((char *)gl_redcap->bridge->data->buffer, gl_redcap->bridge->data->len,  ' ');
  nbr = 0;
  while (tab[nbr])
    {
      printf("tab:%s\n", tab[nbr]);
      nbr++;
    }
  printf("nbr:%d\n", nbr);
  if (nbr == 3)
    {
      gl_redcap->name = strdup(tab[1]);
      gl_redcap->icon = atoi(tab[2]);
      gl_redcap->engine->server->name = strdup(gl_redcap->name);
      gl_redcap->engine->server->icon = gl_redcap->icon;
    }
  else
    {
      help_set_user(gl_redcap->bridge->fd);
      return;
    }
  send_cli_nfo(SET_CL_USR_INF, gl_redcap->bridge->fd);
}

void	me_info(int fd)
{
  char	*buff;

  buff = malloc((strlen(gl_redcap->name) + 5 + 3)  * MALLOC);
  sprintf(buff, "%s %d\n", gl_redcap->name, gl_redcap->icon);
  if (gl_redcap->bridge->fd != -1)
    Writen(gl_redcap->bridge->fd, buff);
  else
    Writen(fd, buff);
  free(buff);
}
