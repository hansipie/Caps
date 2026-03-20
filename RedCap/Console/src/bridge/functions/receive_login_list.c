#include "../../../includes/redcap.h"

t_field	*tmp_transac(char *val)
{
  t_field               *field, *tmp_field;
  t_field               *ptr;
  u_int16_t		nbr;
  int			i;

  memcpy(&nbr, val, 2);
  nbr = ntohs(nbr);
  val += 2;
  field = NULL;
  for (i = 0; i < nbr; i++) 
    {
      tmp_field = malloc((sizeof(t_field)) * MALLOC);
      tmp_field->type = 0;
      bcopy(val, &tmp_field->id, 2);
      tmp_field->id = ntohs(tmp_field->id);
      val += 2;
      bcopy(val, &tmp_field->size, 2);
      tmp_field->size = ntohs(tmp_field->size);
      val += 2;
      tmp_field->data = malloc((tmp_field->size) * MALLOC);
      bcopy(val, tmp_field->data, tmp_field->size);
      val += tmp_field->size;
      tmp_field->next = NULL;
      ptr = field;
      if (ptr == NULL)
	field = tmp_field;
      else
	{
	  while (ptr->next)
	    ptr = ptr->next;
	  ptr->next = tmp_field;
	}
    }
  return(field);
}

void	receive_login_list(t_transac *transac)
{
  t_field               *field;
  char			*login;

  if (gl_redcap->debug->functions)
    printf("function:receive_login_list\n");
  Writen(gl_redcap->bridge->fd, "\nlogin_list:\n");
  while(transac->field)
    {
      field = tmp_transac(transac->field->data);

      while (field)
	{
	  if (field->id == 105)
	    {
	      Writen(gl_redcap->bridge->fd, "\t");
	      login = malloc(field->size + 2);
	      hl_code(login, field->data, field->size);
	      login[field->size] = 0;
	      Writen(gl_redcap->bridge->fd, login);
	      Writen(gl_redcap->bridge->fd, "\n");
	      free(login);
	    }
	  field = field->next;
	}

      transac->field = transac->field->next;
    }
  prompt(gl_redcap->bridge->fd);
}
