#include "../../includes/redcap.h"

/* ajouter un element a la liste et le chainer a la fin  */
/* BP/s = BUFFER / T(s) */

t_data  *Read_from_bridge(int fd, t_data *data)
{
  int   i;
  char	*buff;
  t_data *new_data;
  int   len;

  
  if (gl_redcap->debug->functions == 1)
    printf("function:read_from_bridge\n");
  buff = malloc((MAXLINE) * MALLOC);
  if ((len = read(fd, buff, MAXLINE)) <= 0)
    {
      if (gl_redcap->debug->other)
        printf("error recv dans Read_from\n");
      new_data = malloc((sizeof(t_data)) * MALLOC);
      new_data->len = -1;
      free(buff);
      return(new_data);
    }
  new_data = malloc((sizeof(t_data)) * MALLOC);
  new_data->next = NULL;
  new_data->len = len;
  new_data->buffer = malloc((len) * MALLOC);
  bcopy(buff, new_data->buffer, new_data->len);
  if (gl_redcap->debug->read == 1)
    {
      printf("recv data of %d bytes :\n", new_data->len);
      for (i = 0; i < new_data->len; i++)
	printf("[0x%x]", new_data->buffer[i]);
      printf("\n");
    }
  if ((new_data->buffer[0] != 13) && (new_data->buffer[1] != 10))
    {
      data = gl_redcap->bridge->data;
      data = insert_data(new_data, data);
      gl_redcap->bridge->data = data;
    }
  free(buff);
  return(data);
}

t_data  *Read_from_server(int fd, t_data *data, t_server *server)
{
  int		err;
  char		*buff;
  t_data	*new_data;
  int		len;
  t_server	*tmp;

  if (gl_redcap->debug->functions == 1)
      printf("function:read_from_server\n");
  buff = malloc((MAXLINE) * MALLOC);
  if((err = pthread_mutex_unlock(&gl_redcap->engine->server->mutex)) != 0)
    printf("unlock in Read_from_server error of gl_redcap->engine->server->mutex\n");
  else
    printf("unlock in Read_from_server succed of gl_redcap->engine->server->mutex\n");
  new_data = malloc((sizeof(t_data)) * MALLOC);
  new_data->next = NULL;
  if ((len = read(fd, buff, MAXLINE)) <= 0)
    {
      if (gl_redcap->debug->other)
        printf("error recv dans Read_from\n");
      new_data->len = -1;
      free(buff);
      return(new_data);
    }
  new_data->len = len;
  new_data->buffer = malloc((new_data->len) * MALLOC);
  bcopy(buff, new_data->buffer, new_data->len);
  if ((err = pthread_mutex_lock(&gl_redcap->engine->server->mutex)) != 0)
    printf("lock in Read_from_server error of gl_redcap->engine->server->mutex\n");
  else
    printf("lock in Read_from_server succed of gl_redcap->engine->server->mutex\n");
  if ((new_data->buffer[0] != 13) && (new_data->buffer[1] != 10))
    {
      tmp = gl_redcap->engine->server;
      while(server != gl_redcap->engine->server)
	gl_redcap->engine->server = gl_redcap->engine->server->next;
      data = gl_redcap->engine->server->data;
      data = insert_data(new_data, data);
      while(server != gl_redcap->engine->server)
	gl_redcap->engine->server = gl_redcap->engine->server->next;
      gl_redcap->engine->server->data = data;
      gl_redcap->engine->server = tmp;
    }
  free(buff);
  return(data);
}
