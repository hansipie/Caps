#include "../../../includes/redcap.h"

void	get_file_info(int command)
{
  t_transac	*transac;
  t_data	*ret, *data, *tmp;

  if(gl_redcap->debug->functions)
    printf("func:get_file_info\n");
  ret = pars_opt(gl_redcap->bridge->data->buffer, 
		 gl_redcap->bridge->data->len, 
		 command);
  tmp = ret;
  if (ret != NULL)
    {
      ret = ret->next;
      transac = malloc((sizeof(t_transac)) * MALLOC);
      transac = build_hdr(transac, GET_FILEI, HDR_RQST);
      transac = build_field_s(transac, FILE_NAME, ret->buffer);
      data = make_path(gl_redcap->engine->server->path, 1);
      if (data)
	transac = build_field_b(transac, FILE_PATH, 
				data->buffer, data->len);      
      send_transac(transac);
    }
  else
    Writen(gl_redcap->bridge->fd, "get_file_info \"my_file\"");
  free_opt(tmp);
}
