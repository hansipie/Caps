#include "../../../includes/redcap.h"

u_int32_t	locate_file(char *name)
{
  DIR           *dirp;
  struct dirent *dirent;
  struct stat   sb;
  char          *path;
  u_int32_t    	len;
  
  if(gl_redcap->debug->functions)
    printf("func:locate_file\n");
  len = 0;
  dirp = opendir(LOCAL_PATH);
  while ((dirent = readdir(dirp)))
    {
      path = my_strcat(LOCAL_PATH, dirent->d_name);
      if (stat(path, &sb) == -1)
	{
	  perror("stat");
	  break;
	}
      if(!strcmp(dirent->d_name, name))
	{
	  len = sb.st_size;
	  closedir(dirp);
	  return(len);
	}      
    }
  closedir(dirp);
  Writen(gl_redcap->bridge->fd, "error: no such file\n");
  return(-1);
}

int	existenz(char *file)
{
  FILE	*pfile;

  pfile = fopen(file, "r");
  if (pfile != NULL)
    {
      fclose(pfile);
      return(1);
    }
  return(0);
}

void    upload(int command)
{
  t_transac	*transac;
  t_data	*data, *tmp, *pars;
  u_int32_t    	len;
  char		*path;
  struct stat   sb;


  if(gl_redcap->debug->functions)
    printf("func:upload\n");
  pars = pars_opt(gl_redcap->bridge->data->buffer, 
		  gl_redcap->bridge->data->len, command);
  tmp = pars;
  if (pars != NULL)
    {
      pars = pars->next;
      path = my_strcat(LOCAL_PATH, pars->buffer);
      if (existenz(path))
	{
	  if (stat(path, &sb) == -1)
	    {
	      perror("stat");
	      return;
	    }
	  len = sb.st_size;
	  transac = malloc((sizeof(t_transac)) * MALLOC);
	  transac = build_hdr(transac, UPL_FILE, HDR_RQST);
	  transac = build_field_s(transac, FILE_NAME, pars->buffer);
	  transac = build_field_i(transac, FILE_TRANSFERT_SIZE, strlen(pars->buffer) + len + 130);
	  data = make_path(gl_redcap->engine->server->path, 1);
	  new_elem_task(pars->buffer, transac->header->id, UPL_FILE, strlen(pars->buffer) + len + 130, 0);
	  if (data)
	    transac = build_field_b(transac, FILE_PATH, data->buffer, data->len);
	  send_transac(transac);
	}	
      else
	Writen(gl_redcap->bridge->fd, "error: no such file\n");
    }
  else
    Writen(gl_redcap->bridge->fd, "upload \"my_file\"\n");
  free_opt(tmp);
}
