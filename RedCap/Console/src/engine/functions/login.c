#include "../../../includes/redcap.h"

void    send_login(char *usr, char *passwd, int fd_connect)
{

  t_transac *transac;
  char  *nusr;
  char  *npass;

  if(gl_redcap->debug->functions)
    printf("function:send_login\n");
  if (!passwd)
      passwd = strdup("");
  nusr = malloc((strlen(usr)) * MALLOC);
  hl_code(nusr, usr, strlen(usr));
  npass = malloc((strlen(passwd)) * MALLOC);
  hl_code(npass, passwd, strlen(passwd));
  transac = malloc((sizeof(t_transac)) * MALLOC);
  transac = build_hdr(transac, LOGIN, HDR_RQST);
  transac = build_field(transac, 105, nusr, strlen(usr), STRG);
  transac = build_field(transac, 106, npass, strlen(passwd), STRG);
  transac = build_field_i(transac, 160, 190);
  send_transac(transac);
  free(passwd);
  free(nusr);
  free(npass);
}

void	main_login(t_server *server)
{
  if(gl_redcap->debug->functions)
    printf("function:main_login\nlogin:'%s'\npasswd:'%s'\nname:'%s'\nicon:'%d'\n", 
	   server->login, server->passwd, gl_redcap->name, gl_redcap->icon);
  send_login(server->login, server->passwd, server->fd);
}
