#include "../../../includes/redcap.h"

void	my_mkdir(int command)
{
  t_transac	*transac;
  t_data        *pars, *data, *tmp;

  if(gl_redcap->debug->functions)
    printf("func:mkdir\n"); 
  pars = pars_opt(gl_redcap->bridge->data->buffer, 
		  gl_redcap->bridge->data->len,
		  command);
  tmp = pars;
  if (pars != NULL)
    {
      pars = pars->next;
      transac = malloc((sizeof(t_transac)) * MALLOC);
      transac = build_hdr(transac, MKDIR, HDR_RQST);
      transac = build_field_s(transac, FILE_NAME, pars->buffer);
      data = make_path(gl_redcap->engine->server->path, 1);
      if (data)
	transac = build_field_b(transac, FILE_PATH, data->buffer, data->len);      
      send_transac(transac);
    }
  else
    Writen(gl_redcap->bridge->fd, "mkdir \"folder\"\n");
  free_opt(tmp);
}

void    lmkdir(int command)
{
  t_data        *pars, *tmp;
  char		*path;

  if(gl_redcap->debug->functions)
    printf("func:lmkdir\n"); 
  pars = pars_opt(gl_redcap->bridge->data->buffer, 
		  gl_redcap->bridge->data->len,
		  command);
  tmp = pars;
  if (pars != NULL)
    {
      pars = pars->next;
      path = my_strcat("./download/", pars->buffer);
      mkdir(path, 0222);
      free(path);
    }
  else
    Writen(gl_redcap->bridge->fd, "lmkdir \"folder\"\n");
  free_opt(tmp);
}
