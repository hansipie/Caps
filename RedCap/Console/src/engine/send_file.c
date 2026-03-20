#include "../../includes/redcap.h"

#define MY_READ_SIZE 512000

int		send_sendfile(int ffd, t_thread_data *transfert, t_data *data)
{
  struct	tms	time_l;
  clock_t	start, stop;
  char		*buf, *tmp, *save;
  int		nb_bits, i;
  long          current_bits;

  if (gl_redcap->debug->functions)
    printf("function:send_sendfile\n");
  nb_bits = 0;
  i = 0;
  buf = malloc(MY_READ_SIZE);
  start = times(&time_l);
  current_bits = 0;
  transfert->current_size = 0;
  transfert->status = strdup("going on");
  transfert->flow = NULL;
  while (((nb_bits = read(ffd, buf, MY_READ_SIZE)) > 0))
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
	  if (transfert->flow)
	    free(transfert->flow);
	  transfert->flow = calc_debit(current_bits, stop - start);
	  current_bits = 0;
	  start = times(&time_l);
	}
      if (i == 0)
	{
	  tmp = malloc(nb_bits + data->len);
	  save = tmp;
	  bcopy(data->buffer, tmp, data->len);
	  tmp += data->len;
	}
      else
	{
	  tmp = malloc(nb_bits);
	  save = tmp;
	}
      bcopy(buf, tmp, nb_bits);
      if(write(transfert->fd, save, nb_bits + data->len) == -1)
	{
	  perror("error in: send_sendfile:");
	  break;
	}
      data->len = 0;
      free(save);
      free(buf);
      buf = malloc(MY_READ_SIZE);
      i++;
    }
  free(buf);
/*   free(data->buffer); */
/*   free(data); */
  return(0);
}

void	send_file(t_thread_data *transfert, t_data *data)
{
  int ffd;
  char	*tmp;

  tmp = my_strcat(LOCAL_PATH, transfert->info->name);
/*   tmp = strdup(transfert->info->name); */
  if ((ffd = open(tmp, O_RDONLY, 0)) < 0)
    perror("open");
  else
    {
      send_sendfile(ffd, transfert, data);
      if (close(ffd) == -1)
	perror("close");
    }
  free(tmp);
}
