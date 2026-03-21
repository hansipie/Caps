#include "../../../includes/redcap.h"

void	get_login_list(int command)
{
  t_transac	*transac;
  t_data        *pars;

  if(gl_redcap->debug->functions)
    printf("func:get_login_list\n"); 
  pars = pars_opt(gl_redcap->bridge->data->buffer, 
		  gl_redcap->bridge->data->len,
		  command);
  if (pars != NULL)
    {
      transac = malloc((sizeof(t_transac)) * MALLOC);
      transac = build_hdr(transac, 348, HDR_RQST);
      send_transac(transac);
    }
  else
    Writen(gl_redcap->bridge->fd, "get_login_list\n");
  free_opt(pars);
}
