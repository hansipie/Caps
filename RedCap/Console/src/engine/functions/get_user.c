#include "../../../includes/redcap.h"

void	get_user(int command)
{
  t_transac	*transac;
  t_data        *pars;

  if(gl_redcap->debug->functions)
    printf("func:get_user\n"); 
  if((pars = pars_opt(gl_redcap->bridge->data->buffer, 
		      gl_redcap->bridge->data->len,
		      command)) != NULL)
    {
      printf("youuu on est la\n");
      pars = pars->next;
      transac = malloc((sizeof(t_transac)) * MALLOC);
      transac = build_hdr(transac, GET_USR, HDR_RQST);
      transac = build_field_s(transac, ID_USR_LOG, pars->buffer);
      send_transac(transac);
    }
  else
    Writen(gl_redcap->bridge->fd, "get_user user_login\n");
}
