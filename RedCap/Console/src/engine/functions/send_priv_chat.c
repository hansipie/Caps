#include "../../../includes/redcap.h"

char	*pars_opt_send_priv_chat(char *buff, int len)
{
  char	**tab;
  int	i;
  char *buff2;

  i = 0;
  buff2 = malloc((len + 1) * MALLOC);
  strncpy(buff2, buff, len);
  tab = str_to_tab(buff2, len, ' ');
  while(tab[i])
    i++;
  if (i < 3 
      || strcmp(tab[0], "send_priv_chat")
      || atoi(tab[1]) < 1
      || len <= strlen(tab[0]) + strlen(tab[1]) + 4
      || buff2[strlen(tab[0]) + strlen(tab[1]) + 2] != '\"'
      || buff2[len - 1] != '\"'
      )
    {
      return(NULL);
    }
  buff2[strlen(buff2) - 1] = 0;
  i = strlen(tab[0]) + strlen(tab[1]) + 3;
  str_to_tab_clean(tab);
  return (buff2 + i);
}

void		main_send_priv_chat(int command)
{
  t_transac	*transac;
  char		*buff, *tmpbuf;
  int		i,j;
  char		**tab;

  if(gl_redcap->debug->functions)
    printf("func:send_priv_chat\n");
  buff = malloc((gl_redcap->bridge->data->len + 10) * MALLOC);
  bcopy(gl_redcap->bridge->data->buffer, buff, gl_redcap->bridge->data->len);
  buff[gl_redcap->bridge->data->len] = 0;
  tab = str_to_tab(buff, gl_redcap->bridge->data->len, ' ');
  for(i = 0; tab[i];)
    i++;
  if (i < 3 
      || strcmp(tab[0], "send_priv_chat")
      || (j = atoi(tab[1])) < 1
      || gl_redcap->bridge->data->len <= strlen(tab[0]) + strlen(tab[1]) + 4
      || buff[strlen(tab[0]) + strlen(tab[1]) + 2] != '\"'
      || buff[gl_redcap->bridge->data->len - 1] != '\"'
      )
    {
      help_send_priv_chat(gl_redcap->bridge->fd);
      return;
    }
  i = strlen(tab[0]) + strlen(tab[1]) + 3;
  buff[gl_redcap->bridge->data->len - 1] = 0;
  transac = malloc((sizeof(t_transac)) * MALLOC);
  transac = build_hdr(transac, SEND_MSG, HDR_RQST);
  transac = build_field_i(transac, ID_UID, j);
  tmpbuf = buff + i;
  transac = build_field_b(transac, ID_DATA, tmpbuf, strlen(tmpbuf));
  send_transac(transac);
  free(buff);
  str_to_tab_clean(tab); 
}
