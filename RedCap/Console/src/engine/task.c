#include "../../../includes/redcap.h"

void	task()
{
  char	*str;

  if (gl_redcap->debug->functions == 1)
    printf("function:task\n");
  Writen(gl_redcap->bridge->fd, "\nset_client_files_list:\n");
  str = malloc((8) * MALLOC);
  sprintf(str, "%d\n", (int)gl_redcap->engine->transac->data->len);
  Writen(gl_redcap->bridge->fd, str);
  free(str);
}
