#include "../../../includes/redcap.h"

void	set_user_edit(int command)
{
  t_transac	*transac;
  char		*nusr, *usr, *access, *npass;
  t_data	*ret, *tmp;
  int		i;

  if(gl_redcap->debug->functions)
    printf("func:set_user_edit\n");
  ret = pars_opt(gl_redcap->bridge->data->buffer, 
		 gl_redcap->bridge->data->len,
		 command);
  tmp = ret;
  if (ret != NULL)
    {
      ret = ret->next;
      transac = malloc((sizeof(t_transac)) * MALLOC);
      transac = build_hdr(transac, SET_USR, HDR_RQST);
      usr = strdup(ret->buffer);
      nusr = malloc(strlen(usr) * MALLOC);
      hl_code(nusr, usr, strlen(usr));
      transac = build_field(transac, 105, nusr, strlen(usr), STRG);
      ret = ret->next->next;
      npass = malloc((strlen(ret->buffer)) * MALLOC);
      hl_code(npass, ret->buffer, strlen(ret->buffer));
      if ((strlen(ret->buffer) == 1) && (ret->buffer[0] == ' '))
	transac = build_field_s(transac, 106, "");
      else
	transac = build_field(transac, 106, npass, strlen(ret->buffer), STRG);
      ret = ret->next->next;
      transac = build_field_s(transac, ID_USR_NAME, ret->buffer);
      ret = ret->next->next;
      for (i = 0; i < strlen(ret->buffer); i++)
	if ((ret->buffer[i] != '0') && (ret->buffer[i] != '1'))
	  break;
      if(i == 64)
	{	
	  access = bitsfield(ret->buffer);
	  transac = build_field_b(transac, ID_USR_ACCESS, access, 8);
	  send_transac(transac);
	}
      else
	Writen(gl_redcap->bridge->fd, "set_user_edit \"user_login\" \"user_password\" \"user_name\" \"user_access\"\n");
      free(nusr);
      free(npass);
    }
  else
    Writen(gl_redcap->bridge->fd, "set_user_edit \"user_login\" \"user_password\" \"user_name\" \"user_access\"\n");
  free_opt(tmp);
}
