#include "../../../includes/redcap.h"

void	show_task_header()
{
  Writen(gl_redcap->bridge->fd,
	   "Type\tTransfert\tSize\t\tSpeed\t\tRate\tProgress\t\tStatus\t\tFile\tRef\n");
}

char	*show_progress(int rate)
{
  if (rate >= 100)
    return("xxxxxxxxxxxxxxxxxxxx");
  if (rate >= 95)
    return("xxxxxxxxxxxxxxxxxxx.");
  if (rate >= 90)
    return("xxxxxxxxxxxxxxxxxx..");
  if (rate >= 85)
    return("xxxxxxxxxxxxxxxxx...");
  if (rate >= 80)
    return("xxxxxxxxxxxxxxxx....");
  if (rate >= 75)
    return("xxxxxxxxxxxxxxx.....");
  if (rate >= 70)
    return("xxxxxxxxxxxxxx......");
  if (rate >= 65)
    return("xxxxxxxxxxxxx.......");
  if (rate >= 60)
    return("xxxxxxxxxxxx........");
  if (rate >= 55)
    return("xxxxxxxxxxx.........");
  if (rate >= 50)
    return("xxxxxxxxxx..........");
  if (rate >= 45)
    return("xxxxxxxxx...........");
  if (rate >= 40)
    return("xxxxxxxx............");
  if (rate >= 35)
    return("xxxxxxx.............");
  if (rate >= 30)
    return("xxxxxx..............");
  if (rate >= 25)
    return("xxxxx...............");
  if (rate >= 20)
    return("xxxx................");
  if (rate >= 15)
    return("xxx.................");
  if (rate >= 10)
    return("xx..................");
  if (rate >= 5)
    return("x...................");
  if (rate < 5)
    return("....................");
  return("..error.............");
}

void	show_task(t_thread_data *tmp)
{
  char		*s, *current_size, *total_size, *vers;
  float		rate;

  current_size = convert(tmp->current_size);
  total_size = convert(tmp->file_size);
  if (tmp->up_down == DWNL_FILE)
    vers = strdup("dwnl");
  if (tmp->up_down == UPL_FILE)
    vers = strdup("upld");
  if (tmp->flow == NULL)
    tmp->flow = strdup("0b");
  s = malloc(40 + 10 + 60 + 70);
  rate = (tmp->current_size * 100.0) / tmp->file_size;
  sprintf(s,
	  "%s\t%s\t%s\t%s/s\t%.1f%%\t[%s]\t%s\t%s\t%d\n",
	  vers, current_size, total_size, tmp->flow, rate, show_progress(rate), tmp->status, tmp->name, tmp->reference);
  free(current_size);
  free(total_size);
  Writen(gl_redcap->bridge->fd, s);
  free(s);
}

void	task(int command)
{
  t_thread_data *thread;
  t_data	*ret;

  if (gl_redcap->debug->functions == 1)
    printf("function:task\n");
  ret = pars_opt(gl_redcap->bridge->data->buffer, 
		 gl_redcap->bridge->data->len, 
		 command);
  if (ret != NULL)
    {
      free_opt(ret);
      Writen(gl_redcap->bridge->fd, "task:\n");
      thread = gl_redcap->engine->server->transfert;
      if (thread)
	{
	  show_task_header();
	  show_task(gl_redcap->engine->server->transfert);
	  thread = gl_redcap->engine->server->transfert;
	  while (thread->next->id != gl_redcap->engine->server->transfert->id)
	    {
	      show_task(thread->next);
	      thread = thread->next;
	    }
	}
      else
	Writen(gl_redcap->bridge->fd, "no task\n");
    }
  else
    help_task(gl_redcap->bridge->fd);
}
