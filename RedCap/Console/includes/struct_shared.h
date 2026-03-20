/* */
/* *** structure shared *** */

typedef struct          s_data
{
  u_int32_t		time; /* tmps mis pour la reception du buffer */
  u_int32_t		len; /* longueur du buffer  */
  char			*buffer; /*le buffer */
  char			*access; /*le buffer */
  char			*login; /*le buffer */
  char			*pass; /*le buffer */
  char			*name; /*le buffer */
  u_int32_t		data_size;
  struct s_data		*next; /* buffer suivant au cas ou le tps de traitement 
				  des donnees serait plus long que le temps de recption du buffer suivant  */
}			t_data;

/* structure de pointeur sur fonction */
typedef struct          s_function
{
  char                  *function;
  void                  (*function_fct)();
  int			argc;
  int			space;
  int			status;
}                       t_function;
