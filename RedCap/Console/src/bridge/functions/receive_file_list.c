#include "../../../includes/redcap.h"

void	  receive_file_list_header()
{
  Writen(gl_redcap->bridge->fd, "Type\tCreator\tSize\t\t?\tScript\tLen\tName\n");
}

void	receive_file_list(t_transac *transac)
{
  char		*buff, *tmp, *tmp2, *str, *name, type[5], creator[5], unknow[4], script[4], *size_str;
  u_int32_t	filesize;
  u_int16_t     size;
  
  Writen(gl_redcap->bridge->fd, "\nset_files_list:\n");
  receive_file_list_header();
  while ((transac->field) && (transac->field->id == 200))
    {
      buff = malloc((transac->field->size) * MALLOC);
      tmp = buff;
      bcopy(transac->field->data, buff, transac->field->size);
      strncpy(type, tmp, 4);
      type[4] = 0;
      tmp += 4;
      strncpy(creator, tmp, 4);
      creator[4] = 0;
      if (!strcmp(creator,""))
	strcpy(creator,"N\\A");
      tmp += 4;
      bcopy(tmp, &filesize, 4);
      filesize = ntohl(filesize);
      size_str = convert(filesize);
      tmp += 4;
      bcopy(tmp, &unknow, 4);
      tmp += 4;
      bcopy(tmp, &script, 2);
      tmp += 2;
      bcopy(tmp, &size, 2);
      size = ntohs(size);
      tmp += 2;
      name = malloc((size+1) * MALLOC);
      tmp2 = name;
      name = strncpy(name, tmp, size);
      name[size] = 0;
      str = malloc((2*transac->field->size) * MALLOC);
      sprintf(str, "%s\t%s\t%s\t\tX\tX\t%d\t%s\n", type, creator, size_str, size,  name);
      free(tmp2);
      Writen(gl_redcap->bridge->fd, str);
      free(buff);
      free(str);
      free(size_str);
      transac->field = transac->field->next;
    }
  prompt(gl_redcap->bridge->fd);
}
