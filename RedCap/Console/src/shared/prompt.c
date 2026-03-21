#include "../../includes/redcap.h"

void	prompt(int fd)
{
  char *str;

  str = malloc((4) * MALLOC);
  if (gl_redcap->debug->functions)
    printf("function:prompt\n");
  Writen(fd, "[");
/*   printf("gl_redcap: %p\n", gl_redcap); */
/*   printf("gl_redcap->engine: %p\n", gl_redcap->engine); */
/*   printf("gl_redcap->engine->server: %p\n", gl_redcap->engine->server); */
/*   printf("len: %d\n", (int)strlen(gl_redcap->engine->server->server_name)); */
/*   printf("Server name: [%s]\n", gl_redcap->engine->server->server_name); */
  if (gl_redcap->engine->server->server_name && strcmp(gl_redcap->engine->server->server_name, "no_name"))
    Writen(fd, gl_redcap->engine->server->server_name);
  else
    {
      if (gl_redcap->engine->server->host)
	Writen(fd, gl_redcap->engine->server->host);
      else
	Writen(fd, "no_host");
    }
  Writen(fd, " ");
  sprintf(str, "%d", gl_redcap->engine->server->id);
  Writen(fd, str);
  free (str);
  Writen(fd, "] % ");
  if (gl_redcap->debug->functions)
    printf("function:prompt fin\n");
}
