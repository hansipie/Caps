#include "../../../includes/redcap.h"

void	help_get_file_info(int command)
{
  if(gl_redcap->debug->functions)
    printf("func:help_get_file_info\n");  
  Writen(gl_redcap->bridge->fd, "file_info \"my_file\"\n");
}

void	cd_help(int command)
{
  if(gl_redcap->debug->functions)
    printf("func:cd_help\n");  
  Writen(gl_redcap->bridge->fd, "cd \"my folder\"\n");
}

void	help_connect(int command)
{
  if(gl_redcap->debug->functions)
    printf("func:help_connect\n");  
  Writen(gl_redcap->bridge->fd, "connect login[:password]@host[:port] (auto_agree)\n");
}


void	help_set_user(int command)
{
  if(gl_redcap->debug->functions)
    printf("func:help_set_user\n");  
  Writen(gl_redcap->bridge->fd, "set_user name icon\n");
}

void	help_send_broadcast(int command)
{
  if(gl_redcap->debug->functions)
    printf("func:help send_broadcast\n");  
  Writen(gl_redcap->bridge->fd, "send_broadcast \"my message\"\n");
  return;
}

void	help_send_chat(int command)
{
  if(gl_redcap->debug->functions)
    printf("func:help send_chat\n");  
  Writen(gl_redcap->bridge->fd, "send_chat \"my chat message\"\n");
  return;
}

void	help_send_priv_chat(int command)
{
  if(gl_redcap->debug->functions)
    printf("func:help_send_priv_chat\n");  
  Writen(gl_redcap->bridge->fd, "send_priv_chat user_id \"my chat message\"\n");
  return;
}

void    help_task(int command)
{
  if(gl_redcap->debug->functions)
    printf("func:help_task\n");  
  Writen(gl_redcap->bridge->fd, "task\n");
  return;
}
