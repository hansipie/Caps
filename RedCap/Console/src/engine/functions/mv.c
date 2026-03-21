#include "../../../includes/redcap.h"

void	mv(int command)
{
  t_data        *pars, *data, *tmp;
  t_transac	*transac;
  char		*path;

  if(gl_redcap->debug->functions)
    printf("func:mv\n"); 
  pars = pars_opt(gl_redcap->bridge->data->buffer, 
		  gl_redcap->bridge->data->len,
		  command);
  tmp = pars;
  if (pars != NULL)
    {      
      pars = pars->next;
      transac = malloc((sizeof(t_transac)) * MALLOC);
      transac = build_hdr(transac, SET_FILE_INFO, HDR_RQST);
      transac = build_field_s(transac, FILE_NAME, pars->buffer);
      path = my_strcat(gl_redcap->engine->server->path, pars->buffer);
      data = make_path(path, 2);
      transac = build_field_b(transac, FILE_PATH, data->buffer, data->len);
      pars = pars->next->next;
      transac = build_field_s(transac, FILE_NEW_NAME, pars->buffer);
      send_transac(transac);
    }
  else
    Writen(gl_redcap->bridge->fd, "mv \"/my/path/fichier\" \"my/new/path/fichier\"\n");
  free_opt(tmp);
}    
