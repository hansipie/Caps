#include "../../includes/redcap.h"

void Writen(int fd, char *ptr)
{
  int		len/* , i */;

  len = strlen(ptr);
  if (gl_redcap->debug->functions)
    printf("functions:Writen\n");
  if (gl_redcap->debug->read == 1)
    {
/*       printf("send data of %d bytes :\n", len); */
/*       for (i = 0; i < len; i++) */
/* 	printf("[0x%x]", ptr[i]); */
/*       printf("\n"); */
    }
  if (write(fd, ptr, len) != len)
    err_sys("writen error");
}

