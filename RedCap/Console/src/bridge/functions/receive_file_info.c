#include "../../../includes/redcap.h"

void	receive_type(char *rec, int len)
{
  char  *str;

  if (gl_redcap->debug->functions)
    printf("function:receive_type\n");
  str = malloc(len + 2);
  memcpy(str, rec, len);
  str[len] = 0;
  Writen(gl_redcap->bridge->fd, str);
  free(str);
}

void	receive_date(char *name, int len)
{
  if (gl_redcap->debug->functions)
    printf("function:receive_date\n");
  Writen(gl_redcap->bridge->fd, "no date");
}

void	receive_file_size(char *tmp, u_int16_t len)
{
  u_int32_t	size;
  char		*str, *str2;

  if (gl_redcap->debug->functions)
    printf("function:receive_file_size\n");
  size = 0;
  memcpy(&size, tmp, len);
  size = ntohs(size);
  str = convert(size);
  str2 = malloc(strlen(str) + 20);
  printf("size:%dl size %d\n", size, len);
  sprintf(str2, "%db %s", size, str);
  Writen(gl_redcap->bridge->fd, str2);
  free(str);
  free(str2);
}

void	receive_type_string(char *name, int len)
{
  char	*str;

  if (gl_redcap->debug->functions)
    printf("function:receive_type_string\n");
  str = malloc(len + 1);
  memcpy(str, name, len);
  str[len] = 0;
  Writen(gl_redcap->bridge->fd, str);
  free(str);
}

void	receive_file_info(t_transac *transac)
{
  if (gl_redcap->debug->functions)
    printf("function:receive_file_info\n");
  Writen(gl_redcap->bridge->fd, "\nfile_info:\n");
  while (transac->field)
    {
      if (transac->field->id == 201)
	{
	  Writen(gl_redcap->bridge->fd, "File name:\t");
	  receive_type_string(transac->field->data, transac->field->size);
	}
      if (transac->field->id == 205)
	{
	  Writen(gl_redcap->bridge->fd, "File type:\t");
	  receive_type_string(transac->field->data, transac->field->size);
	}
      if (transac->field->id == 206)
	{
	  Writen(gl_redcap->bridge->fd, "File creator:\t");
	  receive_type_string(transac->field->data, transac->field->size);
	}
      if (transac->field->id == 210)
	{
	  printf("file comment\n");
	  Writen(gl_redcap->bridge->fd, "Comment:\t");
	  receive_type_string(transac->field->data, transac->field->size);
	}
      if (transac->field->id == 213)
	{
	  Writen(gl_redcap->bridge->fd, "Type:\t\t");
	  receive_type(transac->field->data, transac->field->size);
	}
      if (transac->field->id == 208)
	{
	  Writen(gl_redcap->bridge->fd, "Creator Date:\t");
	  receive_date(transac->field->data, transac->field->size);
	}
      if (transac->field->id == 209)
	{
	  Writen(gl_redcap->bridge->fd, "Modify Date:\t");
	  receive_date(transac->field->data, transac->field->size);
	}
      if (transac->field->id == 207)
	{
	  Writen(gl_redcap->bridge->fd, "File Size:\t");
	  receive_file_size(transac->field->data, transac->field->size);
	}
      Writen(gl_redcap->bridge->fd, "\n");
      transac->field = transac->field->next;
    }
  prompt(gl_redcap->bridge->fd);
}
