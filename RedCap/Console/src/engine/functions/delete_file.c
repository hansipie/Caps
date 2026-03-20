#include "../../../includes/redcap.h"

void	delete_file(int command)
{
  t_transac	*transac;
  t_data        *pars, *data, *tmp;

  if(gl_redcap->debug->functions)
    printf("func:delete_file\n"); 
  pars = pars_opt(gl_redcap->bridge->data->buffer, 
		  gl_redcap->bridge->data->len,
		  command);
  tmp = pars;
  if (pars != NULL)
    {
      pars = pars->next;
      transac = malloc((sizeof(t_transac)) * MALLOC);
      transac = build_hdr(transac, DELETE_FILE, HDR_RQST);
      transac = build_field_s(transac, FILE_NAME, pars->buffer);
      data = make_path(gl_redcap->engine->server->path, 1);
      if (data)
	transac = build_field_b(transac, FILE_PATH, data->buffer, data->len);      
      send_transac(transac);
      remove_data(data);
    }
  else
    Writen(gl_redcap->bridge->fd, "delete_file \"folder\"\n");
  free_opt(tmp);
}
