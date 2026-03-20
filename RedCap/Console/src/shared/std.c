#include "../../includes/redcap.h"

int	is_host(char *str)
{
  int	n;

  for (n = 0; str[n]; n++)
    if (str[n] == '.')
      return (1);
  return (0);
}

char	*my_strcat(char *s1, char *s2)
{
  char	*s3;
  int	n;
  int	n2;

  if (gl_redcap->debug->functions == 1)
    printf("function:my_strcat\n");
  s3 = malloc((strlen(s1) + strlen(s2) + 1) * MALLOC);
  for (n = 0; s1[n]; n++)
    s3[n] = s1[n];
  for (n2 = 0; s2[n2]; n2++)
    s3[n + n2] = s2[n2];
  s3[n + n2] = 0;
  return (s3); 
}

char *my_strndup(char *str, int len)
{
  char *fin;
  int n;

  if (gl_redcap->debug->functions == 1)
    printf("function:my_strndup\n");
  fin = malloc((len + 1) * MALLOC);
  for (n = 0; n < len && str[n]; n++)
    fin[n] = str[n];
  fin[n] = 0;
  return (fin);
}

void	str_to_tab_clean(char **tab)
{
  int	i;

  if (gl_redcap->debug->functions == 1)
    printf("function:str_to_tab_clean\n");
  i = 0;
  while(tab[i])
    {
      free(tab[i]);
      i++;
    }
  free(tab);
}

char **str_to_tab(char *str, int len, char sep)
{
  int n;
  int n2;
  int ntab = 0;
  char **tab;

  if (gl_redcap->debug->functions == 1)
    printf("function:str_to_tab\n");
  tab = malloc(((len + 1) * sizeof(char *)) * MALLOC);
  tab[0] = 0;
  n = 0;
  for (; (n < len) && (str[n] != sep); n++)
    ; 
  printf("len:%d n:%d\n", len, n);
  if (n == len)
    {
      tab[0] = my_strndup(str, len);
      tab[0][len] = 0;
      tab[1] = 0;
      printf("tab[0]:'%s' tab[1]:'%s'\n", tab[0], tab[1]);
      return (tab);
    }
  n = 0;
  while (n < len)//str[n])
    {
      for (; n < len && str[n] == sep; n++)
	;
      for (n2 = n; n2 < len && str[n2] != sep; n2++)
	;
      if (n2 - n)
	{
	  tab[ntab] = my_strndup(str + n, n2 - n);
	  n = n2;
	  ntab++;
	}
      else
	tab[ntab] = 0;
    }
  printf("tab[0]:%s\n", tab[0]);
  tab[ntab] = 0;
  return (tab);
}
