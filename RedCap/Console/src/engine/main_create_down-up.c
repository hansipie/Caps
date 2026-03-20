#include "../../includes/redcap.h"

void	HTXF(t_thread_data *transfert)
{
  char		*buff, *tp;
  u_int32_t	tmp;
  struct dirent *dirent;
  struct stat   sb;
  DIR           *dirp;

  if(gl_redcap->debug->functions)
    printf("func:HTXF\n");
  buff = malloc((16) * MALLOC);
  tp = buff;
  buff = strncpy(buff, "HTXF", 4);
  buff += 4;
  printf("ref:%d\n", transfert->reference);
  transfert->reference = htonl(transfert->reference);
  bcopy(&transfert->reference, buff, sizeof(u_int32_t));
  buff += sizeof(u_int32_t);
  if (transfert->up_down == UPL_FILE)
    {
      dirp = opendir(".");
      while ((dirent = readdir(dirp)))
	{
	  if(!strcmp(dirent->d_name, transfert->name))
	    {
	      stat(dirent->d_name, &sb);
	      transfert->fork->data_size = sb.st_size;
	      transfert->file_size = sb.st_size;
	      break;
	    }
	}
      transfert->transfert_size = transfert->fork->data_size;//probleme de taille entre le dowbload et l'upload
    }
  else if ((transfert->up_down == DWNL_FILE) || (transfert->up_down == ID_FILE_TRANSFERT_OPT))
    transfert->transfert_size = 0;//probleme de taille entre le dowbload et l'upload
  tmp = htonl(transfert->transfert_size);
  bcopy(&tmp, buff, 4);
  buff += sizeof(u_int32_t);
  bzero(buff, 4);
  write(transfert->fd, tp, 16);
  free(tp);
}

void	show_transfert(t_thread_data *transfert)
{
  printf("name:%s\nreference:%dl\nfd:%d\nport:%d\n",
	 transfert->name, 
	 transfert->reference,
	 transfert->fd, 
	 transfert->port);
}

void	main_create_download(t_thread_data *transfert)
{
  t_server	*server;
  struct sockaddr_in servaddr;
  t_data	*data;

  if(gl_redcap->debug->functions)
    printf("func:main_create_download\n");
  server = gl_redcap->engine->server;
  transfert->fd = Socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  transfert->port = server->port + 1;
  servaddr.sin_port = htons(transfert->port);
  inet_pton(AF_INET, server->host, &servaddr.sin_addr);
  if (connect(transfert->fd, (SA *) &servaddr, sizeof(servaddr)) < 0)
      perror("connect");
  transfert = fill_flattened_file_object(transfert);
  HTXF(transfert);
  if (transfert->up_down == UPL_FILE)
    {
      data = send_flattened_file_object(transfert);
      send_file(transfert, data);
    }
  else if (transfert->up_down == DWNL_FILE)
    receive_flattened_file_object(transfert);
  else if (transfert->up_down == ID_FILE_TRANSFERT_OPT)
    receive_preview(transfert);
  close(transfert->fd);
  pthread_exit(0);
}
