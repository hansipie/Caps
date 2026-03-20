#include "../../../includes/redcap.h"

typedef struct          s_transac_ret
{
  t_thread_data		*cur, *prev;
}			t_transac_ret;

t_transac_ret	*find_transac(int ref, int fd)
{
  t_thread_data *transac, *save, *prev;
  t_transac_ret	*ret;

  if(gl_redcap->debug->functions)
    printf("func:find_transac\n");
  if((transac = gl_redcap->engine->server->transfert) != NULL)
    {
      save = transac;
      prev = transac;
      while(ref != transac->reference)
	{
	  prev = transac;
	  transac = transac->next;
	  if(save->reference == transac->reference)
	    {
	      Writen(fd, "error: no such task\n");
	      return(NULL);
	    }
	}
      ret = malloc(sizeof(t_transac_ret));
      ret->cur = transac;
      ret->prev = prev;
      return(ret);
    }
  Writen(fd, "task:\nno task\n");
  return(NULL);
}

void	new_elem_task(char *buff, int id, int vers, int transfert_size, int resume)
{
  t_thread_data *new;

  if(gl_redcap->debug->functions)
    printf("func:new_elem_task\n");
  new = malloc((sizeof(t_thread_data)) * MALLOC);
  new->name = malloc((strlen(buff)) * MALLOC);
  new->name = strdup(buff);
  printf("buff:%s\n", new->name);
  new->fd = -42;
  new->id = id;
  new->file_size = transfert_size - 130 - strlen(buff);
  new->reference = random();
  new->up_down = vers;
  new->flow = NULL;
  if (vers == UPL_FILE)
    new->transfert_size = transfert_size;
  if (resume)
    new->resumage = resume;
  else
    new->resumage = 0;
  insert_new_task(new);
}


void		insert_new_task(t_thread_data *new)
{
  int		nbr;
  t_thread_data *tmp, *tmp2;
  t_thread_data *save;
  
  if(gl_redcap->debug->functions)
    printf("func:insert_new_task\n");
  if (gl_redcap->engine->server->transfert != NULL)
    {
      nbr = 1;
      save = gl_redcap->engine->server->transfert;
      tmp = gl_redcap->engine->server->transfert;
      tmp2 = save;
      while (tmp->next->id != save->id)
	{
	  tmp = tmp->next;
	  nbr++;
	}
      if (nbr == 1)
	{
	  save->next = new;
	  new->next = save;
	}
      if (nbr > 1)
	{
	  tmp->next = new;
	  new->next = save;
	}
      gl_redcap->engine->server->transfert = save;
    }
  else
    {
      new->next = new;
      gl_redcap->engine->server->transfert = new;
    }
}

void		stop_task(int command)
{
  t_data        *pars, *tmp;
  t_thread_data *transac;
  t_transac_ret *ret;

  if(gl_redcap->debug->functions)
    printf("func:stop_task\n");
  transac = gl_redcap->engine->server->transfert;
  pars = pars_opt(gl_redcap->bridge->data->buffer, 
		  gl_redcap->bridge->data->len,
		  command);
  tmp = pars;
  if (pars != NULL)
    {
      pars = pars->next;
      if ((ret = find_transac(atol(pars->buffer), gl_redcap->bridge->fd)) != NULL)
	{
	  transac = ret->cur;
	  close(transac->fd);
	  perror("close");
	  free(transac->status);
	  transac->status = strdup("stop");
	}
      else
	return;
    }
  else
    Writen(gl_redcap->bridge->fd, "error: stop_task reference\n");
  free_opt(tmp);
}

void		remove_task(char *vers, long ref, int fd)
{
  t_thread_data *transac, *prev, *save;
  t_transac_ret *ret;  

  if(gl_redcap->debug->functions)
    printf("func:remove_task\n");
  transac = gl_redcap->engine->server->transfert;
  save = transac;
  if ((ret = find_transac(ref, fd)) != NULL)
    {
      transac = ret->cur;
      prev = ret->prev;
      close(transac->fd);     
      /* faire le free */
      if (!strncmp(vers, "cancel_task", 11))
	unlink(transac->file_path);
      if (ret->cur->reference == ret->prev->reference)
	{
	  if (ret->cur->next->reference != ret->cur->reference)
	    {
	      save = ret->cur->next;
	      save->next = ret->cur->next;
	    }
	  else
	    save = NULL;
	}
      else
	prev->next = transac->next;
      gl_redcap->engine->server->transfert = save;
    }
  return;  
}

void	modify_task(char *buff, int id, int vers, int transfert_size, int resume)
{
  t_thread_data *transac, *save;

  if(gl_redcap->debug->functions)
    printf("func:modify_task\n");
  transac = gl_redcap->engine->server->transfert;
  save = transac;
  if(transac != NULL)
    {
      if (!strcmp(transac->name, buff))
	{
	  remove_task("modify_task", transac->reference, transac->fd);
	  gl_redcap->engine->server->transfert = save->next;
	  return;
	}	
      if (transac->next)
	transac = transac->next;
      while(save->reference != transac->reference)
	{
	  if (!strcmp(transac->name, buff))
	    {
	      remove_task("modify_task", transac->reference, transac->fd);
	      gl_redcap->engine->server->transfert = save;
	      return;
	    }
	  transac = transac->next;
	}
    }
}

void		delete_task(int command)
{
  t_data        *pars, *tmp;

  if(gl_redcap->debug->functions)
    printf("func:delete_task\n");
  pars = pars_opt(gl_redcap->bridge->data->buffer,
		  gl_redcap->bridge->data->len,
		  command);
  if (pars != NULL)
    {
      pars = pars->next;
      remove_task("delete_task", atol(pars->buffer), gl_redcap->bridge->fd);
    }
  else
    Writen(gl_redcap->bridge->fd, "error: delete_task reference\n");
  free_opt(tmp);
}

void		cancel_task(int command)
{
  t_data        *pars, *tmp;
  
  if(gl_redcap->debug->functions)
    printf("func:cancel_task\n");
  tmp = pars;
  pars = pars_opt(gl_redcap->bridge->data->buffer, 
		  gl_redcap->bridge->data->len,
		  command);
  if (pars != NULL)
    {
      pars = pars->next;
      remove_task("cancel_task", atol(pars->buffer), gl_redcap->bridge->fd);
    }
  else
    Writen(gl_redcap->bridge->fd, "error: cancel_task reference\n");
  free_opt(tmp);
}
