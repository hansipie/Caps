#include "../../../includes/redcap.h"

void	lrename(int command)
{
  t_data        *pars, *tmp;
  char		*path, *path2;

  if(gl_redcap->debug->functions)
    printf("func:lmv\n");
  pars = pars_opt(gl_redcap->bridge->data->buffer, 
		  gl_redcap->bridge->data->len,
		  command);
  tmp = pars;
  if (pars != NULL)
    {
      pars = pars->next;
      path = my_strcat("./download/", pars->buffer);
      printf("old_path:%s pars->buffer:%s\n", path, pars->buffer);
      pars = pars->next->next;
      path2 = my_strcat("./download/", pars->buffer);
      printf("new_path:%s pars->buffer:%s\n", path2, pars->buffer);
      rename(path, path2);
      free(path);
      free(path2);
    }
  else
    Writen(gl_redcap->bridge->fd, "lmv \"my_file\" \"mon fichier\"\n");
  free_opt(tmp);  
}
