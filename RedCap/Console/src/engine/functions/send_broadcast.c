#include "../../../includes/redcap.h"

void		main_send_broadcast(int command)
{
  t_transac	*transac;
  t_data       	*buff, *tmp;

  if(gl_redcap->debug->functions)
    printf("func:send_broadcast\n");
  buff = pars_opt(gl_redcap->bridge->data->buffer, 
		  gl_redcap->bridge->data->len, 
		  command);
  tmp = buff;
  if (buff != NULL)
    {
      buff = buff->next;
      transac = malloc((sizeof(t_transac)) * MALLOC);
      transac = build_hdr(transac, SEND_BROADCAST, HDR_RQST);
      transac = build_field_s(transac, ID_DATA, buff->buffer);
      send_transac(transac);
    }
  else
    help_send_broadcast(gl_redcap->bridge->fd);
  free_opt(tmp);
}
