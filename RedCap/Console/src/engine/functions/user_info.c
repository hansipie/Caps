#include "../../../includes/redcap.h"

void	user_info(int command)
{
  t_transac	*transac;
  t_data        *pars, *tmp;
  int		id;

  if(gl_redcap->debug->functions)
    printf("func:user_info\n"); 
  pars = pars_opt(gl_redcap->bridge->data->buffer, 
		  gl_redcap->bridge->data->len,
		  command);
  tmp = pars;
  if (pars != NULL)
    {
      pars = pars->next;
      transac = malloc((sizeof(t_transac)) * MALLOC);
      transac = build_hdr(transac, GET_CL_INF_TXT, HDR_RQST);
      id = atoi(pars->buffer);
      printf("id:%d\n", id);
      transac = build_field_i(transac, ID_UID, id);
      send_transac(transac);
    }
  else
    Writen(gl_redcap->bridge->fd, "user_info user_id\n");
  free_opt(tmp);
}
