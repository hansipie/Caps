#include "../../../includes/redcap.h"

void	delete_user(int command)
{
  t_transac	*transac;
  char		*nusr, *usr;
  t_data	*ret, *tmp;

  if(gl_redcap->debug->functions)
    printf("func:delete_user\n");
  ret = pars_opt(gl_redcap->bridge->data->buffer, 
		 gl_redcap->bridge->data->len, 
		 command);
  tmp = ret;
  if (ret != NULL)
    {
      ret = ret->next;
      transac = malloc((sizeof(t_transac)) * MALLOC);
      transac = build_hdr(transac, DELETE_USR, HDR_RQST);
      usr = strdup(ret->buffer);
      nusr = malloc(strlen(usr) * MALLOC);
      hl_code(nusr, usr, strlen(usr));
      transac = build_field(transac, 105, nusr, strlen(usr), STRG);      
      send_transac(transac);
      free(nusr);
    }
  else
    Writen(gl_redcap->bridge->fd, "delete_user user_login\n");
  free_opt(tmp);
}
