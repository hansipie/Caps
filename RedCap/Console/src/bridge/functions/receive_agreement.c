#include "../../../includes/redcap.h"

void	receive_agreement(t_transac *transac)
{
  char	*buff;
  int	i;

  if (gl_redcap->debug->functions)
    printf("function:receive_agreement\n");
  if (transac->nbr_param == 0)
    {
      Writen(gl_redcap->bridge->fd, "\nno_agreement");
      //      if(!gl_redcap->engine->server->auto_agree)
	agreed(gl_redcap->bridge->fd);
      Writen(gl_redcap->bridge->fd, "\n");
      prompt(gl_redcap->bridge->fd);
      return;
    }
  if (transac->field->id == 154)
    {
      agreed(gl_redcap->bridge->fd);
      prompt(gl_redcap->bridge->fd);
    return;
    }
  if (transac->field->id == 101)
    {
      if (gl_redcap->debug->functions)
        printf("Agreement received with ID 101\n");
/*       if (transac->field->size == 0) */
/* 	return; */
      buff = malloc((transac->field->size + 1) * MALLOC);
      for (i = 0; i < transac->field->size; i++)
	{
	  buff[i] = transac->field->data[i];
	  if (buff[i] == 13)
	    buff[i] = '\n';
	}
      buff[i] = '\0';
      Writen(gl_redcap->bridge->fd, "\nAgreement:\n");
      Writen(gl_redcap->bridge->fd, "--------------------------------------------------\n");
      Writen(gl_redcap->bridge->fd, buff);
      Writen(gl_redcap->bridge->fd, "\n--------------------------------------------------\n");
      free(buff);
      if (gl_redcap->engine->server->auto_agree == 0)
	Writen(gl_redcap->bridge->fd, "agree ? disagree ?\n");
    }
  prompt(gl_redcap->bridge->fd);
}
