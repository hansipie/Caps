#include "../../../includes/redcap.h"

void    lrm(int command)
{
  t_data        *pars, *tmp;
  char		*path;

  if(gl_redcap->debug->functions)
    printf("func:lrm\n");
  pars = pars_opt(gl_redcap->bridge->data->buffer, 
		  gl_redcap->bridge->data->len,
		  command);
  tmp = pars;
  if (pars != NULL)
    {
      pars = pars->next;
      path = my_strcat("./download/", pars->buffer);
      unlink(path);
      free(path);
    }
  else
    Writen(gl_redcap->bridge->fd, "lrm \"my_file\"\n");
  free_opt(tmp);
}
