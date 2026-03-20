#include "../../../includes/redcap.h"

char	*update2(char *buff, int len)
{
  int	i;

  for (i = 0; i <= len; i++)
    if(buff[i] == 13)
      buff[i] = '\n';
  return(buff);
}

void	receive_server_msg(t_transac *transac)
{
  char *buff;
  char *from;
  char *msg;
   u_int16_t id;

  from = NULL;
  msg = NULL;

  if(gl_redcap->debug->functions)
    printf("function:receive_server_msg\n");
  while (transac->field)
    {
      switch (transac->field->id)
	{
	case 103:
	  bcopy(transac->field->data, &id, 2);
	  id = ntohs(id);
	  break;
	case 102:
	  buff = malloc((transac->field->size + 1) * MALLOC);
	  bcopy(transac->field->data, buff, transac->field->size);
	  buff[transac->field->size] = '\0';
	  from = strdup(buff);
	  free(buff);
	  break;
	case 113:
	  break;
	case 101:
	  buff = malloc((transac->field->size + 1) * MALLOC);
	  bcopy(transac->field->data, buff, transac->field->size);
	  buff[transac->field->size] = '\0';
	  msg = strdup(buff);
	  free(buff);
	  break;
	case 214:
	  break;
	default:
	  printf("error: unknown field %d\n", transac->field->id);
	}
      transac->field = transac->field->next;
    }
  if (!from)
    { 
      buff = malloc((strlen("\nreceive_server_msg: ") + strlen(msg) + 2) * MALLOC);
      strcpy(buff, "\nreceive_server_msg: ");
      strcat(buff, msg);
      strcat(buff, "\n");
    }
  else
    {
      buff = malloc((strlen("\nreceive_priv_chat from: ") + strlen(from) + strlen(msg) + strlen(" id: ") + 10) * MALLOC);
      sprintf(buff, "\nreceive_priv_chat from: %s id: %d\n%s\n", from, id, msg);
    }
  Writen(gl_redcap->bridge->fd, buff);
  free(buff);
  prompt(gl_redcap->bridge->fd);
}
