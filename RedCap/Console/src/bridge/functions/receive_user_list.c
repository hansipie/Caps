#include "../../../includes/redcap.h"

char	*get_bin(int flag)
{
  char	*ret;
  int	i;

  i = 0;
  ret = malloc((32) * MALLOC);
  while (flag >= 1)
    {
      ret[i] = (flag / 2) % 2 + 48;
      flag = flag / 2;
      i++;
    }
  ret[i] = 0;
  return(ret);
}

char	*user_flags(char *flag)
{
  int	rpm, rpc, type, away;
  char	*str;

/* 		Noth	admin	RPC	RPM	coadmin	AWAY */
/* 		10	11	12	13	14	15 */
/* 0000000000	[0]	[1]	[0]	[0]	[1]	[0]	admin */
/* 0000000000	[0]	[0]	[0]	[0]	[1]	[0]	co-admin */
/* 0000000000	[0]	[0]	[0]	[0]	[0]	[0]	guest */
/* 0000000000	[1]	[0]	[0]	[0]	[0]	[0]	noth */
/* 0000000000	[1]	[0]	[0]	[1]	[0]	[0]	noth, refuse private message */
/* 0000000000	[1]	[0]	[1]	[1]	[0]	[0]	noth, refuse private message, refuse private chat */

  rpm = 0;
  rpc = 0;
  type = 1;
  away = 0;
  if (flag[10] == '1')
    type = 0;
  if (flag[12] == '1')
    rpc = 1;
  if (flag[13] == '1')
    rpm = 1;
  if (flag[14] == '1')
    type = 2;
  if (flag[11] == '1')
    type = 3;  
  if (flag[15] == '1')
    away = 1;
  free(flag);
  str = malloc(20);
  sprintf(str, "%d    %d    %d    %d", rpm, rpc, type, away);
  return(str);
}

void	show_user_header(int vers)
{
  if (vers == 1)
    Writen(gl_redcap->bridge->fd, "\nuser_list:\n");
  if (vers == 2)
    Writen(gl_redcap->bridge->fd, "\nnotify change user:\n");
  Writen(gl_redcap->bridge->fd,
         "id\ticon\tflag\t\t\tsize\tname\n\t\tRpm  Rpc  Type Away\n");
}

char	*func_uflag(char *user_flag)
{
  char *str, *str2;
  int	i, tmp;

  str2 = malloc(65);
  str2 = strcpy(str2, "");
  for (i = 0; i < 2; i++)
    {
      str = malloc(5);
      sprintf(str, "%d", user_flag[i]);
      tmp = atoi(str);
      strcat(str2, c_to_b(tmp));
      free(str);
    }
  str2 = user_flags(str2);
  return(str2);
}

void	receive_user_list(t_transac *transac)
{
  char	*buff, *tmp, *name, *user_flag;
  u_int16_t	user_id, icon, size;
  char *str;
  if (gl_redcap->debug->functions)
    printf("function:receive_User_list\n");
  show_user_header(1);
  while ((transac->field) && (transac->field->id == 300))
    {
      buff = malloc((transac->field->size) * MALLOC);
      bcopy(transac->field->data, buff, transac->field->size);
      tmp = buff;
      bcopy(tmp, &user_id, 2);
      user_id = ntohs(user_id);
      tmp+=2;
      bcopy(tmp, &icon, 2);
      icon = ntohs(icon);
      tmp+=2;
      user_flag = func_uflag(tmp);
      tmp+=2;
      bcopy(tmp, &size, 2);
      size = ntohs(size);
      tmp+=2;
      name = malloc((size+1) * MALLOC);
      name = strncpy(name, tmp, size);
      name[size] = 0;
      str = malloc((21 + size + strlen(user_flag)) * MALLOC);
      sprintf(str, "%d\t%d\t%s\t\t%d\t%s\n",
	      user_id, icon, user_flag, size, name);
      Writen(gl_redcap->bridge->fd, str);
      free(name);
      free(buff);
      free(str);
      free(user_flag);
      transac->field = transac->field->next;
    }  
  prompt(gl_redcap->bridge->fd);
}

void      notify_change_user(t_transac *transac)
{
  char		*buff, *name, *tmp, *flag;
  u_int16_t     id, icon_id;  
  int		len;

  if (gl_redcap->debug->functions)
    printf("function:notify_change_user\n");
  show_user_header(2);
  while(transac->field)
    {
      switch(transac->field->id)
	{
	case 103:
	  memcpy(&id, transac->field->data, 2);
	  id = ntohs(id);
	  break;
	case 104:
	  memcpy(&icon_id, transac->field->data, 2);
	  icon_id = ntohs(icon_id);
	  break;
	case 112:
	  flag = malloc(transac->field->size + 1);
	  memcpy(flag, transac->field->data, transac->field->size);
	  flag = func_uflag(flag);
	  break;
	case 102:
	  buff = malloc((transac->field->size + 1) * MALLOC);
	  memcpy(buff, transac->field->data, transac->field->size);
	  buff[transac->field->size] = '\0';
	  name = strdup(buff);
	  free(buff);
	  break;
	default:
	  printf("error: unknown field %d\n", transac->field->id);
	}
      transac->field = transac->field->next;
    }
  len = strlen(name);
  tmp = strdup(flag);
  free(flag);
  buff = malloc(((sizeof(u_int16_t) * 2) + strlen(tmp) + sizeof(int) + sizeof(char) * len + 7) * MALLOC);
  sprintf(buff, "%d\t%d\t%s\t\t%d\t%s\n", id, icon_id, tmp, len, name);
  Writen(gl_redcap->bridge->fd, buff);
  prompt(gl_redcap->bridge->fd);
  free(buff);
}
