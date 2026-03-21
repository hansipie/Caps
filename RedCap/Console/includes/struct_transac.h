/* */
/* *** structures de transactions *** */

/* structure header */
typedef struct		s_hl_hdr
{
  u_int16_t		grbg[2];
  u_int32_t		id;
  u_int32_t		err;
  u_int32_t		total_len;
  u_int32_t		data_len;
}			t_hl_hdr;

/* structure field */
typedef	struct		s_field
{
  u_int16_t		id;
  u_int16_t		size;
  int			type;
  char			*data;
  struct s_field	*next;
}			t_field;

/* structure transaction */
typedef struct		s_transac
{
  t_hl_hdr		*header;
  u_int16_t		nbr_param;
  t_field		*field;
}			t_transac;
