#include "../includes/redcap.h"

/* ouverture et parsage du fichier de conf */
void    pars_conf_file()
{
  int fd;
  char **tab;
  char *str;

  fd = open("./redcap.conf", O_RDONLY, 0755);
  if ((fd == -1) && (gl_redcap->debug))
    perror("pars_conf_file(): ");
  else
    {
      while((str = get_next_line(fd)))
        {
          if (*str && str[0] != '#')
            {
              tab = str_to_tab(str, strlen(str), ' ');
              if (!strcmp(tab[0], "name"))
                gl_redcap->name = strdup(tab[1]);
              if (!strcmp(tab[0], "icon"))
                gl_redcap->icon = atoi(tab[1]);
              if (!strcmp(tab[0], "local_port"))
                gl_redcap->bridge->port = atoi(tab[1]);
              str_to_tab_clean(tab);
            }
          free(str);
        }
      close(fd);
    }
}

void	init_default_serv()
{
  gl_redcap->engine = malloc((sizeof(t_engine)) * MALLOC);
  gl_redcap->engine->server = malloc((sizeof(t_server)) * MALLOC);
  gl_redcap->engine->server->data = NULL;
  gl_redcap->engine->server->id = 0;
  gl_redcap->engine->server->host = strdup("Client RedCap");
  gl_redcap->engine->server->server_name = strdup("no_name");
  gl_redcap->engine->server->connected = 0;
  gl_redcap->engine->server->port = 0;
  gl_redcap->engine->server->path = strdup("/");
  gl_redcap->engine->server->auto_agree = 0;
  gl_redcap->engine->server->login = strdup("\0");
  gl_redcap->engine->server->passwd = strdup("\0");
  gl_redcap->engine->server->thread = NULL;
  gl_redcap->engine->server->transfert = NULL;
  gl_redcap->engine->server->next = gl_redcap->engine->server;
  gl_redcap->engine->server->prev = gl_redcap->engine->server;
  pthread_mutex_init(&gl_redcap->engine->mutex, NULL);
  perror("pthread_mutex_init in init_default_serv: ");
  gl_redcap->bridge = malloc((sizeof(t_bridge)) * MALLOC);
  gl_redcap->bridge->data = NULL;
  gl_redcap->bridge->port = 5505;
  gl_redcap->bridge->fd = -1;
  gl_redcap->bridge->passwd = NULL;
  gl_redcap->bridge->nbr = 0;
}

void	init_conf()
{
  init_default_serv();
  pars_conf_file();
}
