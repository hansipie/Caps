#include "../../includes/redcap.h" 

int     xwrite(int fd, void *buff, int len)
{
  int res;

  res = write(fd, buff, len);
  return (res);
}

void    hl_code (void *__dst, const void *__src, size_t len)
{
  u_int8_t *dst = (u_int8_t *)__dst, *src = (u_int8_t *)__src;
  

  for (; len; len--)
    *dst++ = ~*src++;  
}
