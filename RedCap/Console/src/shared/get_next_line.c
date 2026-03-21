#include "../../includes/redcap.h"
#define READ_SIZE2       42

char    *get_next_line(int fd)
{
  static char   buffer[READ_SIZE2];
  static int    pos = 0;
  static int    size = -1;
 
  int           pos2;
  char          *s2;
  char          *s3;
  char          *s4;

  if (size < 0 || pos >= size)
    {
      if (!(size = read(fd,buffer,READ_SIZE2)))
        return (0);
      pos = 0;
    }
  pos2 = pos;
  while (42)
    {
      if (pos2>=size)
        {
          s2 = malloc((pos2-pos+1) * MALLOC);
          strncpy(s2,buffer+pos,pos2-pos);
          s2[pos2-pos] = 0;
          size = -1;
          if ((s3 = get_next_line(fd)))
            {
              s4 = malloc((strlen(s2)+strlen(s3)+1) * MALLOC);
              strncpy(s4,s2,strlen(s2));
              strncpy(s4+strlen(s2),s3,strlen(s3));
              s4[strlen(s2)+strlen(s3)] = 0;
              free(s2);
              free(s3);
              return (s4);
            }
          else
            return (s2);
        }
      if (*(buffer+pos2)=='\n')
        {
          s2 = malloc((pos2-pos+1) * MALLOC);
          strncpy(s2,buffer+pos,pos2-pos);
          s2[pos2-pos] = 0;
          pos = pos2+1;
          return (s2);
        }
      pos2 ++;
    }
}
