#include "../../../includes/redcap.h"

void	get_user_list(int command)
{
  t_transac *transac;

  if(gl_redcap->debug->functions)
    printf("func:get_user_list\n");
  transac = malloc((sizeof(t_transac)) * MALLOC);
  transac = build_hdr(transac, GET_UNAME_LST, HDR_RQST);
  send_transac(transac);
}
