#include "../../../includes/redcap.h"

char	*update(char *buff, int len)
{
  int	i, nbr;

  nbr = 0;
  for (i = 0; i <= len; i++)
    {
      if(buff[i] == 13)
	{
	if(nbr)
	  buff[i] = '\n';
	nbr++;
	}
    }
  return(buff);
}

void	receive_chat(t_transac *transac)
{
  char	*buff;
  
  buff = malloc((transac->field->size + 1) * MALLOC);
  bcopy(transac->field->data, buff, transac->field->size);
  buff[transac->field->size] = '\0';
  Writen(gl_redcap->bridge->fd, "\nreceive_chat:\n");
  buff = update(buff, transac->field->size);
  Writen(gl_redcap->bridge->fd, buff);
  Writen(gl_redcap->bridge->fd, "\n");
  free(buff);
  prompt(gl_redcap->bridge->fd);
}
