#include "../../../includes/redcap.h"

void	receive_broadcast(t_transac *transac)
{
  char	*buff;
  char *from;
  char *msg;

  from = NULL;
  msg = NULL;

  if(gl_redcap->debug->functions)
    printf("function:receive_broadcast\n");
  while (transac->field)
    {
      switch (transac->field->id)
	{
	case 103:
	  break;
	case 102:
	  buff = malloc((transac->field->size + 1) * MALLOC);
	  bcopy(transac->field->data, buff, transac->field->size);
	  buff[transac->field->size] = '\0';
	  from = strdup(buff);
	  free(buff);
	  break;
	case 101:
	  buff = malloc((transac->field->size + 1) * MALLOC);
	  bcopy(transac->field->data, buff, transac->field->size);
	  buff[transac->field->size] = '\0';
	  msg = strdup(buff);
	  free(buff);
	  break;
	default:
	  printf("error: unknown field %d\n", transac->field->id);
	}
      transac->field = transac->field->next;
    }
  buff = malloc((strlen("broadcast_msg from: ") + strlen(from) + strlen(msg) + 4) * MALLOC);
  sprintf(buff, "\nbroadcast_msg from: %s\n%s\n", from, msg);
  Writen(gl_redcap->bridge->fd, buff);
  free(buff);
  prompt(gl_redcap->bridge->fd);
}
