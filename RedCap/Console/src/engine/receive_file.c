#include "../../includes/redcap.h"

#define MY_READ_SIZE 512000

t_data	remove_flat_truc(char *buf)
{
  u_int16_t	name_size;
  u_int32_t	data_size;
  t_data	data;

  buf += 110;
  bcopy(buf, &name_size, sizeof(u_int16_t));
  name_size = ntohs(name_size);
  data.len = name_size;
  buf += (name_size + 16);
  bcopy(buf, &data_size, sizeof(u_int32_t));
  data.data_size = ntohl(data_size);
  buf += 4;
  data.buffer = buf;
  data.time = 110 + name_size + 16 + 4;
  return(data);
}

char	*convert(float t)
{
  int	i;
  char	*s;

  if(gl_redcap->debug->functions)
    printf("func:convert\n");
  i = 0;
  while (t > 1024)
    {
      t /= 1024;
      i++;
      if (i >= 4)
	break;
    }
  s = malloc(30 * MALLOC);  
  if (i == 0)
    sprintf(s, "%.1f octets", (float)t);
  if (i == 1)
    sprintf(s, "%.1f ko", (float)t);
  if (i == 2)
    sprintf(s, "%.1f Mo", (float)t);
  if (i == 3)
    sprintf(s, "%.1f Go", (float)t);
  if (i == 4)
    sprintf(s, "%.1f To", (float)t);
  return(s);
}

char	*calc_debit(long current_bytes, float cycle)
{
  float	time;
  float	debit;
  int	i;
  char	*str;

  if(gl_redcap->debug->functions)
    printf("func:calc_debit\n");
  i = 0;
  time = cycle / sysconf(_SC_CLK_TCK);
  debit = current_bytes / time;
  str = convert(debit);  
  return(str);
}

void	receive_preview(t_thread_data *transfert)
{
  long	tot;
  long	nb_bits;
  char  *buf, *save, *name;
  char	*tmp;
  int	i, ffd, flag;

  i = 0;
  if(gl_redcap->debug->functions)
    printf("func:receive_preview\n");
  tot = 0;
  Writen(gl_redcap->bridge->fd, "\npreview:\n");
  buf = malloc(20);
  save = buf;
  flag = O_CREAT | O_EXCL | O_RDWR;//
  name = my_strcat("preview_", transfert->name);
  ffd = open(name, flag, 0600);//
  while (((nb_bits = read(transfert->fd, buf, 20)) > 0))
    {
      write(ffd, buf, nb_bits);//
      printf("nb_bits:%ld\n", nb_bits);
      tot = tot + nb_bits;
      if (nb_bits < 20)
	break;
    }
  close(ffd);
  tmp = malloc(40);
  sprintf(tmp, "receive:%ld\n", tot);
  Writen(gl_redcap->bridge->fd, tmp);
  free(tmp);
  nb_bits = 0;
  if ((ffd = open(name, O_RDONLY, 0)) < 0)
    perror("open:");
  while (((nb_bits = read(ffd, buf, 20)) > 0))
    write(gl_redcap->bridge->fd, buf, nb_bits);
  unlink(name);
  free(name);
  Writen(gl_redcap->bridge->fd, "\n\n--------------\nend of preview\n");
  remove_task("remove_task", transfert->reference, transfert->fd);
  prompt(gl_redcap->bridge->fd);
}

void	receive_file(t_thread_data *transfert)
{
  struct	tms	time_l;
  clock_t	start, stop;
  char		buf[MY_READ_SIZE], *name, *path;
  int		nb_bits, i, ffd, flag;
  t_data	data;
  long		current_bits, transfert_bits;
  
  if (gl_redcap->debug->functions)
    printf("function:receive_file\n");
  i = 0;
  transfert_bits = 0;
  name = strdup(transfert->name);
  name = my_strcat(name, ".hpf");
  if (transfert->resumage)
    flag = O_APPEND | O_EXCL | O_RDWR;
  else
    flag = O_CREAT | O_EXCL | O_RDWR;
  path = my_strcat(LOCAL_PATH, name);
  printf("path to open:%s\n",path);
  if ((ffd = open(path, flag, 0600)) < 0)
    {
      perror("open");
      Writen(gl_redcap->bridge->fd, "\nDownload: File ");
      Writen(gl_redcap->bridge->fd, transfert->info->name);
      Writen(gl_redcap->bridge->fd, " already downloaded\n");
      transfert->status = strdup("error");
      transfert->flow = strdup("0.0 octets");
      prompt(gl_redcap->bridge->fd);
      return;
    }
  start = times(&time_l);
  current_bits = 0;
  transfert->current_size = 0;
  transfert->status = strdup("going on");
  transfert->flow = 0;
  while (((nb_bits = read(transfert->fd, buf, MY_READ_SIZE)) > 0))
    {
      current_bits += nb_bits;
      transfert->current_size += nb_bits;
      stop = times(&time_l);
      if ((stop - start) >= 15)
	{
	  current_bits = 0;
	  start = times(&time_l);
	}
      else if ((stop - start) >= 10)
	{
	  transfert->flow = calc_debit(current_bits, stop - start);
	  current_bits = 0;
	  start = times(&time_l);
	}
      if (i != 0)
	{
	  write(ffd, buf, nb_bits);
	  memset(buf, 0, nb_bits);
	}
      else
	{
	  data = remove_flat_truc(buf);
	  write(ffd, data.buffer, (nb_bits - (data.len + 130)));
	  transfert->file_size = data.data_size;
	  transfert->current_size -= data.time;
	  i = 1;
	}
      if (transfert->current_size >= transfert->file_size)
	{
	  transfert->status = strdup("complete");
	  transfert->flow = strdup("0b");
	  //	  name = strdup(transfert->name);
	  //transfert->name[strlen(transfert->name) - 4] = 0;
	  name = my_strcat(LOCAL_PATH, transfert->name);
	  //transfert->name = my_strcat(LOCAL_PATH, transfert->name);
	  if (rename(path, name) == -1)
	    perror("rename error");
	}
    }
  transfert->flow = 0;
  if (close(ffd) == -1)
    {
      perror("close");
      printf("[%s]\n", strerror(errno));
    }
}
