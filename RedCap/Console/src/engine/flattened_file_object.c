#include "../../includes/redcap.h"

t_thread_data *fill_flat_file_header(t_thread_data *transfert)
{
  transfert->header = malloc((sizeof(t_flat_file)) * MALLOC);
  transfert->header->format  = malloc((4) * MALLOC);
  transfert->header->format = strncpy(transfert->header->format, "FILP", 4);
  transfert->header->version = 1;
  transfert->header->rsvd = malloc((16) * MALLOC);
  bzero(transfert->header->rsvd, 16);
  transfert->header->fork_count = 2;
  return(transfert);
}

t_thread_data *fill_flat_file_information_fork_header(t_thread_data *transfert)
{
  printf("debug:%s\n", transfert->name);
  transfert->info_fork = malloc((sizeof(t_flat_file_info)) * MALLOC);
  transfert->info_fork->fork_type = strdup("INFO");
  transfert->info_fork->compression_type = 0;
  transfert->info_fork->rsvd = malloc((4) * MALLOC);
  bzero(transfert->header->rsvd, 4);
  transfert->info_fork->data_size = (strlen(transfert->name) + 72 + 2);
  return(transfert);
}

t_thread_data *fill_flat_file_information_fork(t_thread_data *transfert)
{
  transfert->info = malloc((sizeof(t_flat_file_info_fork)) * MALLOC);
#ifdef __i386__
  transfert->info->plateform = strdup("MWIN");
#endif
#ifdef __ppc__
  transfert->info->plateform = strdup("AMAC");
#endif  
  transfert->info->type_signature = strdup("TEXT");
  transfert->info->creator_signature = strdup("ttxt");
  transfert->info->flags = 0;
  transfert->info->plateform_flags = 0;
  transfert->info->rsvd = malloc((32) * MALLOC);
  bzero(transfert->info->rsvd, 32);
  transfert->info->create_date = malloc((8) * MALLOC);
  bzero(transfert->info->create_date, 8);
  transfert->info->modify_date = malloc((8) * MALLOC);
  bzero(transfert->info->modify_date, 8);
  transfert->info->name_script = 0;
  transfert->info->name_size = strlen(transfert->name);
  transfert->info->name = strdup(gl_redcap->engine->server->transfert->name);
  return(transfert);
}

t_thread_data *fill_flat_file_fork_header(t_thread_data *transfert)
{  
  transfert->fork = malloc((sizeof(t_flat_file_info)) * MALLOC);
  transfert->fork->fork_type = strdup("DATA");
  transfert->fork->compression_type = 0;
  transfert->fork->rsvd = malloc((4) * MALLOC);
  bzero(transfert->fork->rsvd, 4);
  transfert->fork->data_size = transfert->transfert_size;
/*   transfert->fork->data_size = 4; */
  return(transfert);
}

char	*send_flat_file_header(t_thread_data *transfert, t_data *data)
{
  int	i;
  char	*buff, *save;

  if(gl_redcap->debug->functions)
    printf("func:send_flat_file_header__fork_header\n");
  buff = malloc((24) * MALLOC);
  save = buff;
  for (i = 0; i < 4; i++)
    buff[i] = transfert->header->format[i];
  buff+=4;
  transfert->header->version = htons(transfert->header->version);
  bcopy(&transfert->header->version, buff, sizeof(u_int16_t));
  buff += sizeof(u_int16_t);
  bzero(buff, 16);
  buff += 16;
  transfert->header->fork_count = htons(transfert->header->fork_count);
  bcopy(&transfert->header->fork_count, buff, sizeof(u_int16_t));
/*   data->buffer = save; */
/*   data->len = 24; */
  //write(transfert->fd, save, 24);
  //free(save);
  return(save);
}

char    *send_flat_file_information_fork_header__fork(t_thread_data *transfert, int vers, t_data *data)
{
  int	i;
  char	*buff, *save;
  u_int32_t	tmp;

  if(gl_redcap->debug->functions)
    printf("func:send_flat_file_information_fork_header\n");  
  buff = malloc((16) * MALLOC);
  save = buff;
  if (vers == 1)
    for (i = 0; i < 4; i++)
      buff[i] = transfert->info_fork->fork_type[i];
  else
    for (i = 0; i < 4; i++)
      buff[i] = transfert->fork->fork_type[i];
  buff += 4;
  bzero(buff, 4);
  buff += 4;
  bzero(buff, 4);
  buff += 4;
  if (vers == 1)
    tmp = htonl(transfert->info_fork->data_size);
  else
    tmp = htonl(transfert->fork->data_size);
  bcopy(&tmp, buff, sizeof(u_int32_t));
/*   write(transfert->fd, save, 16); */
/*   free(save); */
/*   data->buffer = save; */
/*   data->len += 16; */
  return(save);
}

char	*send_flat_file_information_fork(t_thread_data *transfert, t_data *data)
{
  int	i;
  char	*buff, *save;
  
  if(gl_redcap->debug->functions)
    printf("func:send_flat_file_information_fork\n");  
  buff = malloc((72 + strlen(transfert->info->name)) * MALLOC);
  save = buff;
  for (i = 0; i < 4; i++)
    buff[i] = transfert->info->plateform[i];
  buff+=4;
  bcopy(transfert->info->type_signature, buff, 4);
  buff+=4;
  bcopy(transfert->info->creator_signature, buff, 4);
  buff+=4;
  bzero(buff, 4);
  buff+=4;
  bzero(buff, 4);
  buff+=4;
  bzero(buff, 32);
  buff+=32;
  bzero(buff, 8);
  buff+=8;
  bzero(buff, 8);
  buff+=8;
  bzero(buff, 2);
  buff+=2;
  transfert->info->name_size = htons(transfert->info->name_size);
  bcopy(&transfert->info->name_size, buff, sizeof(u_int16_t));
  buff+=(sizeof(u_int16_t));
  bcopy(transfert->info->name, buff, strlen(transfert->info->name));
/*   write(transfert->fd, save, 72 + strlen(transfert->info->name)); */
/*   free(save); */
/*   data->buffer = save; */
/*   data->len += 72; */
  return(save);
}

void	free_transfert(t_thread_data *transfert)
{
  free(transfert->header->format);
  free(transfert->header->rsvd);
  free(transfert->header);

  free(transfert->info_fork->fork_type);
  free(transfert->info_fork->rsvd);
  free(transfert->info_fork);

  free(transfert->info->plateform);
  free(transfert->info->type_signature);
  free(transfert->info->creator_signature);
  free(transfert->info->rsvd);
  free(transfert->info);

  free(transfert->fork->fork_type);
  free(transfert->fork->rsvd);
  free(transfert->fork);
}

t_thread_data *fill_flattened_file_object(t_thread_data *transfert)
{
  if(gl_redcap->debug->functions)
    printf("func:fill_flattened_file_object\n");
  transfert = fill_flat_file_header(transfert);
  transfert = fill_flat_file_information_fork_header(transfert); 
  transfert = fill_flat_file_information_fork(transfert);
  transfert = fill_flat_file_fork_header(transfert);      
  return(transfert);
}

t_data	*send_flattened_file_object(t_thread_data *transfert)
{
  char		*tmp, *tmp2, *save;
  t_data	*data;

  if(gl_redcap->debug->functions)
    printf("func:send_flattened_file_object\n");

  tmp2 = malloc(2);
  bzero(tmp2, 2);

  data = malloc(sizeof(t_data));
  data->buffer = malloc(74 + strlen(transfert->name) + 16 + 16 + 24);
  save = data->buffer;

  tmp = send_flat_file_header(transfert, data);
  bcopy(tmp, data->buffer, 24);
  data->buffer += 24;

  tmp = send_flat_file_information_fork_header__fork(transfert, 1, data);
  bcopy(tmp, data->buffer, 16);
  data->buffer += 16;

  tmp = send_flat_file_information_fork(transfert, data);
  bcopy(tmp, data->buffer, 72 + strlen(transfert->name));
  data->buffer += (72 + strlen(transfert->name));

  bcopy(tmp2, data->buffer, 2);
  data->buffer += 2;

  tmp = send_flat_file_information_fork_header__fork(transfert, 2, data);
  bcopy(tmp, data->buffer, 16);

  data->buffer = save;

  data->len = (74 + strlen(transfert->name) + 16 + 16 + 24);
  return(data);
}

void	receive_flattened_file_object(t_thread_data *transfert)
{
  if(gl_redcap->debug->functions)
    printf("func:receive_flattened_file_object\n");
  receive_file(transfert);
}
