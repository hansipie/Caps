#include "../../../includes/redcap.h"

void	receive_login(t_transac *transac)
{
  char		*buff, *str;
  u_int16_t	vers;  
  t_field	*tempfield;

  if (gl_redcap->debug->functions)
    printf("function:receive_login\n");
  tempfield = transac->field;
  while (tempfield)
    {
      switch (tempfield->id)
	{
	case 160:
	  buff = malloc((tempfield->size) * MALLOC);
	  bcopy(tempfield->data, buff, transac->field->size);
          bcopy(buff, &vers, 2);
          vers = htons(vers);
          str = malloc((8) * MALLOC);
	  sprintf(str, "%d", vers);
          Writen(gl_redcap->bridge->fd, "\nserver version:");
          Writen(gl_redcap->bridge->fd, str);
          Writen(gl_redcap->bridge->fd, "\n");
	  free(str);
          free(buff);
	  break;
	case 161:
	  break;
	case 162:
	  gl_redcap->engine->server->server_name = malloc((tempfield->size + 1) * MALLOC);
	  bcopy(tempfield->data, gl_redcap->engine->server->server_name, tempfield->size);
	  gl_redcap->engine->server->server_name[tempfield->size] = 0;
	  if (gl_redcap->engine->server->server_name)
	    {
	      Writen(gl_redcap->bridge->fd, "\n");
	      Writen(gl_redcap->bridge->fd, "server ");
	      Writen(gl_redcap->bridge->fd, gl_redcap->engine->server->host);
	      Writen(gl_redcap->bridge->fd, " is now know as '");
	      Writen(gl_redcap->bridge->fd, gl_redcap->engine->server->server_name);
	      Writen(gl_redcap->bridge->fd, "'\n");
	      prompt(gl_redcap->bridge->fd);
	    }	  
	  break;
	default:
	  printf("error\n");
	}
      tempfield = tempfield->next;
    }  
  prompt(gl_redcap->bridge->fd);
}
