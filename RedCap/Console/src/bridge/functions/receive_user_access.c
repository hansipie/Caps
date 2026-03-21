#include "../../../includes/redcap.h"

typedef struct          s_access
{
  char                  *access;
}                       t_access;

t_access              gl_access[] =
  {
    { "Delete file" },
    { "Upload file" },
    { "Download file" },
    { "Rename file" },
    { "Move file" },
    { "Create folder" },
    { "Delete folder" },
    { "Rename folder" },
    { "Move folder" },
    { "Read chat" },
    { "Send chat" },
    { "Open chat" },
    { "Close chat" },
    { "Show in list" },
    { "Create user" },
    { "Delete user" },
    { "Open user" },
    { "Modify user" },
    { "Change own password" },
    { "Send private message" },
    { "News read article" },
    { "News post article" },
    { "Disconnect user" },
    { "Cannot be disconnect" },
    { "Get client info" },
    { "Upload anywhere" },
    { "Any name" },
    { "no agreement" },
    { "Set file comment" },
    { "Set folder comment" },
    { "View drop boxes" },
    { "Make alias" },
    { "Broadcast" },
    { "News delete article" },
    { "News create category" },
    { "News delete category" },
    { "News create folder" },
    { "News delete folder" },
    { "Upload folder" },
    { "Download folder" },
    { "Can Send message" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    { "" },
    {0 },
  };


void	show_user_access(char *access, int j)
{
  char	*str;
  int	i;

  if (gl_redcap->debug->functions)
    printf("function:show_user_access\n");
  for (i = 0; (i < 8) && (j <= 40); i++)
    {
      str = malloc(50);      
      sprintf(str, "%s:%c\n", gl_access[j].access, access[i]);
      Writen(gl_redcap->bridge->fd, str);
      free(str);
      j++;
    }
}

char	*c_to_b(unsigned char ti)
{
  int q, r, cpt;
  char *str;
  int   res[8];

  printf("ti:%d ti:0x%x\n", ti, ti);
  cpt = 7;
  do
    {
      q = ti / 2;
      r = ti % 2;
      ti = q;
      if (r == 0)
	res[cpt] = 0;
      else
	res[cpt] = 1;
      printf("q:%d r:%d ti:%d ti:0x%x res[%d]:%d\n", q, r, ti, ti, cpt, res[cpt]);
      cpt--;
    }
  while(ti>0);
  while (cpt >= 0)
    {
      res[cpt] = 0;
      cpt--;
    }
  str = malloc(20);
  sprintf(str, "%d%d%d%d%d%d%d%d", res[0], res[1], res[2], res[3],
	  res[4], res[5], res[6], res[7]);
  return (str);
}

void	receive_user_access(t_transac *transac)
{
  char	*str2;
  int	i, j;
  unsigned char c;

  if (gl_redcap->debug->functions)
    printf("function:receive_user_access\n");  
  j = 0;

  show_transac(transac);
  while ((transac->field) && (transac->field->id))
    {
      if (transac->field->id == 110)
	{
	  Writen(gl_redcap->bridge->fd, "\nmy_access:\n");
	  for (i = 0; i < transac->field->size; i++)
	    {
	      gl_redcap->engine->server->my_access[i] = transac->field->data[i];
	      printf("test: data:[%d] gl_redcap->engine->server->my_access[%d]:", 
		     (unsigned char)transac->field->data[i], (unsigned char)gl_redcap->engine->server->my_access[i]);
	      c = transac->field->data[i];
	      str2 = c_to_b(c);
	      show_user_access(str2, j);
	      free(str2);
	      j+=8;
	    }
	  prompt(gl_redcap->bridge->fd);	  
	}
      transac->field = transac->field->next;
    }
}
