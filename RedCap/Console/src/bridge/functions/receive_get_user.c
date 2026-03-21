#include "../../../includes/redcap.h"

void	receive_get_user(t_transac *transac)
{
  char	*str2, *login, *pass;
  int	i, j;

  if (gl_redcap->debug->functions)
    printf("function:receive_get_user\n");
  j = 0;
  Writen(gl_redcap->bridge->fd, "\nget_user:\n");
  while (transac->field)
    {
      if (transac->field->id == 102)
	{
	  Writen(gl_redcap->bridge->fd, "user_name:\n");
	  receive_type_string(transac->field->data, transac->field->size);
	}
      if (transac->field->id == 105)
	{
	  Writen(gl_redcap->bridge->fd, "user_login:\n");
	  login = malloc(transac->field->size + 2);
	  hl_code(login, transac->field->data, transac->field->size);
	  login[transac->field->size] = 0;
	  Writen(gl_redcap->bridge->fd, login);
	  free(login);
	}
      if (transac->field->id == 106)
	{
	  Writen(gl_redcap->bridge->fd, "user_password:\n");
	  pass = malloc(transac->field->size + 2);
	  hl_code(pass, transac->field->data, transac->field->size);
	  for (i = 0; i < transac->field->size; i++)
	    printf("[%x]", pass[i]);	    
	  free(pass);
	}
      if (transac->field->id == 110)
	{
	  Writen(gl_redcap->bridge->fd, "\nuser_access:\n");
	  for (i = 0; i < transac->field->size; i++)
	    {
/* 	      str = malloc(20); */
	      printf("0x%x\n",transac->field->data[i]);
/* 	      sprintf(str, "%d", transac->field->data[i]); */
/* 	      tmp = atoi(str); */
/* 	      //tmp = ntohs(tmp); */
/* 	      free(str); */
	      str2 = c_to_b(transac->field->data[i]);	      
	      printf("%s\n", str2);
	      show_user_access(str2, j);
	      //	      free(str2);
	      j+=8;
	    }
	  prompt(gl_redcap->bridge->fd);	  
	}
      Writen(gl_redcap->bridge->fd, "\n");
      transac->field = transac->field->next;
    }
  prompt(gl_redcap->bridge->fd);
}
