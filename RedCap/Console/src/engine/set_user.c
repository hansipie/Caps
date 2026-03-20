#include "../../../includes/redcap.h"

void	set_user_edit(int fd)
{
  t_transac	*transac;
  char		*buff, *nusr, *usr, *access, *npass, *passwd;
  t_data	*ret;
  int		access_len, i;

  if(gl_redcap->debug->functions)
    printf("func:set_user\n");
  buff = malloc((gl_redcap->bridge->data->len + 2) * MALLOC);
  buff = memcpy(buff, gl_redcap->bridge->data->buffer,
		gl_redcap->bridge->data->len);
  if((ret = pars_opt(buff, gl_redcap->bridge->data->len,
		     "set_user", 5)) != NULL)
    {
      transac = malloc((sizeof(t_transac)) * MALLOC);
      transac = build_hdr(transac, SET_USR, HDR_RQST);
      usr = strdup(ret->login);
      printf("login:%s\n", usr);
      nusr = malloc(strlen(usr) * MALLOC);
      hl_code(nusr, usr, strlen(usr));
      transac = build_field(transac, 105, nusr, strlen(usr), STRG);
      npass = malloc((strlen(ret->pass)) * MALLOC);
      printf("pass:%s\n", ret->pass);
      hl_code(npass, ret->pass, strlen(ret->pass));
      transac = build_field(transac, 106, npass, strlen(ret->pass), STRG);
      transac = build_field_s(transac, ID_USR_NAME, ret->name);
      access = bitsfield(ret->access);
      printf("access:%s\n", access);
      printf("access:\n");
      for (i = 0; i < 8; i++)
	printf("0x%x ", access[i]);
      printf("\n");
      transac = build_field_b(transac, USER_ACCES, access, 8);
      send_transac(transac);
      free(nusr);
      free(npass);
/*       free_opt(ret); */
    }
  else
    Writen(fd, "set_user \"user_login\" \"user_password\" \"user_name\" \"user_access\"\n");
}
