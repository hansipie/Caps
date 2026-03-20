#include "../../../includes/redcap.h"

t_data		*make_path(char *path, int vers)
{
  int		i, buff_len, sub_dir_nbr;
  u_int16_t	len, temp;
  u_int8_t	unknow;
  t_data	*data;
  char		**tab;
  char		*tmp;

  if(gl_redcap->debug->functions)
    printf("function:make_path\n");
  i = 0;
  unknow = 0;
  sub_dir_nbr = 0;
  tab = str_to_tab(path, strlen(path), '/');
  while(tab[sub_dir_nbr])
    sub_dir_nbr++;
  if (vers == 2)
    sub_dir_nbr--;
  if (sub_dir_nbr >= 0)
    {
      data = malloc(sizeof(t_data) * MALLOC);
      data->buffer = malloc((2 + strlen(path) - sub_dir_nbr + (3 * sub_dir_nbr)) * MALLOC);
      tmp = data->buffer;
      temp = htons(sub_dir_nbr);
      bcopy(&temp, data->buffer, 2);
      data->buffer += 2;
      buff_len = 2;
      if (sub_dir_nbr >= 1)
	  while(i < sub_dir_nbr)
	    {
	      bcopy(&unknow, data->buffer, 1);
	      data->buffer += 1;	  
	      len = strlen(tab[i]);
	      temp = htons(len);
	      bcopy(&temp, data->buffer, 2);
	      data->buffer += 2;
	      bcopy(tab[i], data->buffer, len);
	      data->buffer += len;
	      buff_len += (3 + len);
	      i++;
	    }
      data->len = buff_len;
      data->buffer = tmp;
      data->next = NULL;
      return(data);
    }
  str_to_tab_clean(tab);
  return(NULL);
}

void	get_file_list(int command)
{
  t_transac *transac;
  t_data	*buff;

  if(gl_redcap->debug->functions)
    printf("function:get_file_list\n");
  transac = malloc((sizeof(t_transac)) * MALLOC);
  transac = build_hdr(transac, GET_FNAME_LST, HDR_RQST);
  buff = make_path(gl_redcap->engine->server->path, 1);
  if (buff != NULL)
    {
      transac = build_field_b(transac, FILE_PATH, buff->buffer, buff->len);
      free(buff->buffer);
      free(buff);
    }
  show_transac(transac);
  send_transac(transac);
}
