#include "../../../includes/redcap.h"

void	pwd(int command)
{
  if(gl_redcap->debug->functions)
    printf("func:pwd\n");
  Writen(gl_redcap->bridge->fd, "pwd:\n");
  Writen(gl_redcap->bridge->fd, gl_redcap->engine->server->path);
  Writen(gl_redcap->bridge->fd, "\n");
}
