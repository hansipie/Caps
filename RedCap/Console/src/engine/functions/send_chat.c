#include "../../../includes/redcap.h"

void		main_send_chat(int command)
{
  t_transac	*transac;
  t_data	*ret;

  if(gl_redcap->debug->functions)
    printf("func:main_send_chat\n");
  ret = pars_opt(gl_redcap->bridge->data->buffer, 
		 gl_redcap->bridge->data->len, command);
  if(ret != NULL)
    {
      ret = ret->next;
      printf("ret->buffer:%s\n", ret->buffer);
      transac = malloc((sizeof(t_transac)) * MALLOC);
      transac = build_hdr(transac, SEND_CHAT, HDR_RQST);
      transac = build_field_i(transac, ID_CHAT_OPT, 0);
      transac = build_field_s(transac, ID_DATA, ret->buffer);
      send_transac(transac);
      free_opt(ret);
    }
  else
    help_send_chat(gl_redcap->bridge->fd);
}
