#include "../../includes/redcap.h"

void	show_data_list(t_data *data)
{
  t_data        *tmp;

  tmp = data;
  while(tmp != NULL)
    {
      printf("t_data\n");
      printf("time   : [%d]\n", tmp->time);
      printf("len    : [%d]\n", tmp->len);
      printf("buffer : [%s]\n\n", tmp->buffer);
      tmp = tmp->next;
    }
}

t_data	*remove_data(t_data *data)
{
  t_data        *tmp;

  if (gl_redcap->debug->functions)
    printf("function:remove_data\n");
  tmp = NULL;
  if (data)
    {
      if (data->next)
	tmp = data->next;
      if (data->buffer)
	free(data->buffer);
      free(data);
      return(tmp);
    }
  else
    return(data);
}

void	list_data(t_data *data)
{
  int	i;

  i = 0;
  while (data->next)
    {
      printf("data[%d]:%p\n", i, data);
      i++;
      data = data->next;
    }
}

t_data	*insert_data(t_data *new_data, t_data *data)
{
  t_data	*tmp;

  if (gl_redcap->debug->functions == 1)
    printf("function:insert_data\n");
  if (data == NULL)
    {
      data = new_data;
      data->next = NULL;
    }
  else
    {
      tmp = data;
      while(tmp->next)
	tmp = tmp->next;
      tmp->next = new_data;
      new_data->next = NULL;
    }
  return(data);
}
