#include "../../../includes/redcap.h"

void	my_access(int command)
{
  char  *str2;
  int   i, j;

  j = 0;
  Writen(gl_redcap->bridge->fd, "\nmy_access:\n");
  for (i = 0; i < 8; i++)
    {
      printf("-----> %d\n", i);
      str2 = c_to_b(gl_redcap->engine->server->my_access[i]);
      show_user_access(str2, j);
      free(str2);
      j+=8;
    }
}

