#include "../../../includes/redcap.h"

int	is_here(char *name)
{
  DIR           *dirp;
  struct dirent *dirent;
  struct stat   sb;
  char          *path;
  u_int32_t     len;

  if(gl_redcap->debug->functions)
    printf("func:is_here\n");
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
  return(0);
}

t_data	*build_resume(t_data *resume)
{
  t_flat_file		*resume_data;
  char			*save;

  resume_data = malloc(sizeof(t_flat_file));
  resume_data->fork_info_list = malloc(sizeof(t_fork_info_list));
  resume_data->format = strdup("RFLT");
  resume_data->version = 1;
  resume_data->rsvd = malloc(34);
  bzero(resume_data->rsvd, 34);
  resume_data->fork_count = 2;
  resume_data->fork_info_list->fork = malloc(4 * sizeof(char));
  resume_data->fork_info_list->fork = strdup("DATA");
  resume_data->fork_info_list->data_size = resume->len;
  resume_data->fork_info_list->rsvd_a = malloc(4);
  bzero(resume_data->fork_info_list->rsvd_a, 4);
  resume_data->fork_info_list->rsvd_b = malloc(4);
  bzero(resume_data->fork_info_list->rsvd_b, 4);
  resume->buffer = malloc(58);
  save = resume->buffer;
  bcopy(resume_data->format, resume->buffer, 4);
  resume->buffer += 4;
  resume_data->version = htons(resume_data->version);
  bcopy(&resume_data->version, resume->buffer, 2);
  resume->buffer += 2;
  bzero(resume->buffer, 34);
  resume->buffer += 34;
  resume_data->fork_count = htons(resume_data->fork_count);
  bcopy(&resume_data->fork_count, resume->buffer, 2);
  resume->buffer += 2;  
  bcopy(resume_data->fork_info_list->fork, resume->buffer, 4);
  resume->buffer += 4;
  resume_data->fork_info_list->data_size = htonl(resume_data->fork_info_list->data_size);
  bcopy(&resume_data->fork_info_list->data_size, resume->buffer, 4);
  resume->buffer += 4;
  bzero(resume_data->fork_info_list->rsvd_a, 4);
  resume->buffer += 4;
  bzero(resume_data->fork_info_list->rsvd_b, 4);
  resume->buffer += 4;
  resume->len = 58;
  resume->buffer = save;
  return(resume);
}

void    download(int command)
{
  t_transac	*transac;
  t_data	*pars, *data, *resume, *tmp_pars;
  int		flag, vers, resum;
  char		*tmp;

  if(gl_redcap->debug->functions)
    printf("func:download\n");
  flag = 0;
  resum = 0;
  vers = 0;
  pars = pars_opt(gl_redcap->bridge->data->buffer, 
		  gl_redcap->bridge->data->len, command);
  tmp_pars = pars;
  if (pars != NULL)
    {
      pars = pars->next;
      tmp = pars->buffer;
      if(is_here(tmp))
	{
	  Writen(gl_redcap->bridge->fd, "error: file already download\n");	 
	  free_opt(tmp_pars);
	  return;
	}
      tmp = my_strcat(tmp, ".hpf");
      resume = malloc(sizeof(t_data));
      resume->len = is_here(tmp);
      transac = malloc((sizeof(t_transac)) * MALLOC);
      transac = build_hdr(transac, DWNL_FILE, HDR_RQST);      
      transac = build_field_s(transac, FILE_NAME, pars->buffer);
      data = make_path(gl_redcap->engine->server->path, 1);
      if (data)
	transac = build_field_b(transac, FILE_PATH, 
				data->buffer, data->len);      
      if (resume->len)
	{
	  resume = build_resume(resume);
	  transac = build_field_b(transac, FILE_RESUME_DATA, 
				  resume->buffer, resume->len);
	  resum = 1;
	}
      //suprime une tache non utile
      if (resum == 1)
	{
	  modify_task(pars->buffer, transac->header->id,
		      DWNL_FILE, vers, resum);
	}
      new_elem_task(pars->buffer, transac->header->id, 
		    DWNL_FILE, vers, resum);
      send_transac(transac);      
      
    }
  else
    Writen(gl_redcap->bridge->fd, "download \"my_file\"\n");
  free_opt(tmp_pars);
}
