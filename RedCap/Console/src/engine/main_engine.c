#include "../../includes/redcap.h"

void	main_engine()
{
  t_server	*server;
  t_transac	*transac;
  int		tmp, len,/*i,*/ err, still;
  char		*buff;
  t_data	*tmp_data;

  tmp_data = NULL;  
  still = 0;
  if (gl_redcap->debug->functions)
    printf("function:main_engine\n");
  pthread_cond_init(&gl_redcap->engine->cond, NULL);
  if (gl_redcap->debug->thread)
    perror("pthread_cond_init in main_engine");
  for ( ; ; )
    {		
/*       usleep(1); */
      pthread_mutex_lock(&gl_redcap->engine->mutex);
      printf("still:%d\n", still);
      if(!still)
	{
	  if (gl_redcap->debug->thread)
	    printf("Engine waiting for data...\n");
	  pthread_cond_wait(&gl_redcap->engine->cond, &gl_redcap->engine->mutex);
	  perror("pthread_cond_wait in main_engine");
	}
      else
	printf("pthread_cond_wait pass in main_engine");
      if(still)
	printf("no wait besause still data\n");
      server = gl_redcap->engine->server;      
      if (pthread_mutex_unlock(&gl_redcap->engine->mutex))
	perror("pthread_mutex_unlock in main_engine of gl_redcap->engine->mutex");
       if (tmp_data != server->data)
	tmp_data = server->data;
      if ((server->data) && (server->connected))
	{
	  if (pthread_mutex_trylock(&gl_redcap->engine->server->mutex))
	    perror("pthread_mutex_trylock in Read_from_server of gl_redcap->engine->mutex");
	  tmp = 0;
	  buff = server->data->buffer;
	  len = server->data->len;
	  while ((tmp < server->data->len) && (len > 0))
	    {
	      printf("use transac of size %d from server %s\n", len, server->name);
	      transac = parse_transac(buff, len);
	      use_transac(transac, server);
	      buff += (transac->header->data_len + 20);
	      tmp += (transac->header->data_len + 20);
	    }
	  transac_free(transac);
	  server->data = remove_data(server->data);
	  if (server->data)
	    still = 1;
	  else
	    still = 0;
	  gl_redcap->engine->server->data = server->data;
	  if (pthread_mutex_unlock(&gl_redcap->engine->server->mutex))
	    perror("pthread_mutex_unlock in Read_from_server of gl_redcap->engine->mutex");
	}
      if (server->connected == -1)
	{
	  gl_redcap->engine->server = manage_server(server->thread, server, "extract_server");
	  server = gl_redcap->engine->server;
	  prompt(gl_redcap->bridge->fd);
	  close(server->fd);
	}
    }
  if((err = pthread_cond_destroy(&gl_redcap->engine->cond)) != 0)
    perror("pthread_cond_destroy in Read_from_server of gl_redcap->engine->cond");
}
