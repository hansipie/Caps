#include "../../../includes/redcap.h"


/* t_data	*pars_opt(char *buff, int len_buff, char *func, int nbr_args) */
/* { */
/*   char		**tab; */
/*   int		i, len_func; */
/*   t_data	*ret; */

/*   if(gl_redcap->debug->functions) */
/*     printf("func:pars_opt\n"); */
/*   tab = str_to_tab(buff, len_buff, ' '); */
/*   for (i = 0; tab[i]; i++) */
/* /\*     printf("%s\n", tab[i]) *\/; */
/*   if (i != nbr_args) */
/*     { */
/*       if(gl_redcap->debug->functions) */
/* 	printf("\tpars_opt: args error\n"); */
/*       return(NULL); */
/*     } */
/*   len_func = strlen(func); */
/*   if (nbr_args > 1) */
/*     if ((len_buff <= (len_func + 3))  */
/* 	|| (buff[strlen(func) + 1] != '\"')  */
/* 	|| (buff[len_buff - 1] != '\"')) */
/*       { */
/* 	if(gl_redcap->debug->functions) */
/* 	  printf("\tpars_opt: error\n"); */
/* 	return (NULL); */
/*       } */
/*   ret = malloc((sizeof(t_data)) * MALLOC); */
/*   ret->buffer = malloc((len_buff + 1) * MALLOC); */
/*   if (nbr_args > 1) */
/*     { */
/*       memcpy(ret->buffer, buff + (len_func + 2), len_buff - (len_func + 3) + 1); */
/*       ret->buffer[len_buff - (len_func + 3)] = 0; */
/*     } */
/*   else */
/*     ret->buffer = my_strndup(tab[0], strlen(tab[0])); */
/*   str_to_tab_clean(tab); */
/*   printf("pars_opt ret:[%s]\n", ret->buffer); */
/*   return(ret); */
/* } */

