#include "../../../includes/redcap.h"

void	receive_user_info(t_transac *transac)
{
  int	i;

  if (gl_redcap->debug->functions)
    printf("function:receive_user_info\n");
  Writen(gl_redcap->bridge->fd, "\nuser_info:\n");
  while (transac->field)
    {
      if (transac->field->id == 102)
	{
	  Writen(gl_redcap->bridge->fd, "user_name:\t");
	  receive_type_string(transac->field->data, transac->field->size);
	}
      if (transac->field->id == 101)
	{
	  Writen(gl_redcap->bridge->fd, "user_id:\t");
	  for (i = 0; i < transac->field->size; i++)
	    printf("%c", transac->field->data[i]);
	  printf("\n");
	  receive_type_string(transac->field->data, transac->field->size);
	}
      Writen(gl_redcap->bridge->fd, "\n");
      transac->field = transac->field->next;
    }
  prompt(gl_redcap->bridge->fd);
}
