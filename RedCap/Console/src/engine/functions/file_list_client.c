#include "../../../includes/redcap.h"

void            main_file_list_client(int dummy)
{
  DIR           *dirp;
  struct dirent *dirent;
  struct stat   sb;
  struct passwd *passwd;
  char		*str, *tmp, *str2;
  char		*path;
  int		i;

  i = 0;
  if (gl_redcap->debug->functions == 1)
    printf("function:main_file_list_client\n");
  Writen(gl_redcap->bridge->fd, "\nset_client_files_list:\n");
  receive_file_list_header();
  path = strdup(LOCAL_PATH);  //faire getpwd mettre chemin entier
  printf("path:[%s]\n", path);
  if ((dirp = opendir(path)) == NULL)
    perror("opendir");
  else
    {
      while ((dirent = readdir(dirp)))
	{
	  i++;
	  tmp = malloc(strlen(path) + strlen(dirent->d_name) + 1);
	  tmp = strcpy(tmp, path);
	  tmp = strcat(tmp, dirent->d_name);
	  if ((stat(tmp, &sb)) == -1)
	    perror("stat");
	  else
	    {
	      if(S_ISDIR(sb.st_mode))
		str2 = strdup("fldr\t");
	      else
		str2 = strdup("othr\t");
	      Writen(gl_redcap->bridge->fd, str2);
	      passwd = getpwuid(sb.st_uid);
	      Writen(gl_redcap->bridge->fd, passwd->pw_name);
	      Writen(gl_redcap->bridge->fd, "\t");
	      str = convert(sb.st_size);
	      Writen(gl_redcap->bridge->fd, str);
	      free(str);
	      Writen(gl_redcap->bridge->fd, "\t0\t0\t");
	      str = malloc((8) * MALLOC);
	      sprintf(str, "%d", (int)strlen(dirent->d_name));
	      Writen(gl_redcap->bridge->fd, str);
	      free(str);
	      Writen(gl_redcap->bridge->fd, "\t");
	      Writen(gl_redcap->bridge->fd, dirent->d_name);
	      Writen(gl_redcap->bridge->fd, "\n");
	    }
	  free(tmp);
	}
      closedir(dirp);
    }
}
