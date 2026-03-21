#include "../../../includes/redcap.h"

void	disconnect(int command)
{
/*   t_transac *transac; */
  char	*buff;

/*   transac = malloc((sizeof(t_transac)) * MALLOC); */
/*   transac = build_hdr(transac, 110, HDR_RQST); */
  gl_redcap->engine->server->connected = 2;
  gl_redcap->engine->server->auto_reconnect = 0;
/*   send_transac(transac); */
  write(gl_redcap->engine->server->fd, "", 0);
  shutdown(gl_redcap->engine->server->fd, SHUT_RDWR);  
  perror("shutdown");
/*   buff = malloc((strlen("\ndisconnected from ") + strlen(gl_redcap->engine->server->host) + 3) * MALLOC); */
/*   sprintf(buff, "\ndisconnected from %s\n", gl_redcap->engine->server->host); */
/*   Writen(gl_redcap->bridge->fd, buff); */
/*   free(buff); */
}

void    disconnect_user(int command)
{
  t_transac *transac;

  if(gl_redcap->debug->functions)
    printf("function:disconnect\n");
  if (gl_redcap->engine->server->connected != 3)
    {
      Writen(gl_redcap->bridge->fd, "Connection to server is abording...\n");
      return;
    }
  if (strcmp(gl_redcap->engine->server->host, "Client RedCap"))
    {
      transac = malloc((sizeof(t_transac)) * MALLOC);
      transac = build_hdr(transac, 110, HDR_RQST);
      transac = build_field_i(transac, 103, 0);
      send_transac(transac);      
      gl_redcap->engine->server->connected = 4;
    }
  else
    Writen(gl_redcap->bridge->fd, "Client RedCap is not a real user, you can't delete it.\n");
}
