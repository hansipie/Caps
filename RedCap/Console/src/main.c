#include "../includes/redcap.h"

void	free_init_conf()
{
  free(gl_redcap->engine->server);
  free(gl_redcap->engine);
  free(gl_redcap->bridge);
}


void	free_redcap()
{
  free(gl_redcap->name);
  free(gl_redcap->debug);
}

void	init_redcap()
{
  gl_redcap = malloc((sizeof(t_redcap)) * MALLOC);
  gl_redcap->name = "RedCap";
  gl_redcap->icon = 2001; 
  gl_redcap->engine = NULL;
  gl_redcap->bridge = NULL;
  gl_redcap->debug = malloc((sizeof(t_debug)) * MALLOC);
  gl_redcap->debug->other = 0;
  gl_redcap->debug->read = 0;
  gl_redcap->debug->write = 0;
  gl_redcap->debug->thread = 0;
  gl_redcap->debug->functions = 0;
  gl_redcap->debug->transac = 0;
}

int	main(int argc, char **argv)
{
  pthread_attr_t        *attr;
  pthread_t             *thread;
  int			dummy;

  init_redcap();
  init_conf();
  pars_opts(argc, argv);
  thread = malloc((sizeof(pthread_t)) * MALLOC);
  attr = malloc((sizeof(pthread_attr_t)) * MALLOC);
  if(pthread_attr_init(attr)) 
    perror("pthread_attr_init in main: ");
  if (pthread_mutex_init(&gl_redcap->engine->mutex, NULL)) 
    perror("pthread_mutex_init of gl_redcap->engine->mutex in main: ");
  dummy = 1;
  if (pthread_create(thread, NULL, (void *)main_engine, (void *)dummy))
    perror("pthread_create in main: ");
  main_bridge();  
  free(thread);
  free_redcap();
  return(0);
}
