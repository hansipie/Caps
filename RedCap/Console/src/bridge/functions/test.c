#include <stdlib.h>
#include <stdio.h>

char    *c_to_b(int ti)
{
  int q, r, cpt;
  char c, *str;

/*   if (gl_redcap->debug->functions) */
/*     printf("function:c_to_b\n"); */
  printf("\tentering:%x\n", ti);
  //  str = strdup("00000000");
  str = malloc(9);
  str = strcpy(str, "00000000");
  str[9] = 0;
  cpt = 7;
  do
    {
      q = ti / 2;
      r = ti % 2;
      ti = q;
      c = (r == 0)?'0':'1';
      str[cpt] = c;
      cpt--;
    }
  while(ti>0);
  printf("\toutgoing:%s\n", str);
  return (str);
}

int	main(int argc, char **argv)
{
  char	*str;
  
  str = c_to_b(128);
  printf("str:%s\n", str);
}
