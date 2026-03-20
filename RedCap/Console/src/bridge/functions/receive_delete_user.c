#include "../../../includes/redcap.h"

void	receive_delete_user(t_transac *transac)
{
  u_int16_t id;
  char *buff;
  
  if (gl_redcap->debug->functions)
    printf("function:receive_delete_user\n");
  if(transac->field->id == 103)
    {
      bcopy(transac->field->data, &id, 2);
      id = ntohs(id);
      buff = malloc((29) * MALLOC);
      sprintf(buff, "\nnotify delete user id %d\n", id);
      Writen(gl_redcap->bridge->fd, buff);
      free(buff);
    }
  else
    printf("error: unknown field %d\n", transac->field->id);
  prompt(gl_redcap->bridge->fd);
}
