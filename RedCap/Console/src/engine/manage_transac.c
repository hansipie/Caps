#include "../../includes/redcap.h"

void	launch_reply(t_transac *transac, t_server *server)
{
  if ((transac->header->grbg[1] == 0) && (transac->field))
    if (transac->field->id == 100)
      {
	printf("receive error\n");
	receive_error(transac);
      }
  if ((transac->header->grbg[1] == 0) && (transac->field))
    if (transac->field->id == 107)
      {
	printf("receive upload\n");
	receive_upload(transac);
      }
  if ((transac->header->grbg[1] == 0) && (transac->field))
    if (transac->field->id == 160)
      {
	printf("receive login\n");
	receive_login(transac);
      }
  if ((transac->header->grbg[1] == 0) && (transac->field))
    if (transac->field->id == 108)
      {
	printf("receive download\n");
	receive_download(transac);
      }
  if ((transac->header->grbg[1] == 0) && (transac->field))
    if (transac->field->id == 300)
      {
	printf("receive agreement\n");
	receive_user_list(transac);
      }
  if ((transac->header->grbg[1] == 0) && (transac->field)) 
    if (transac->field->id == 200)
      {
	printf("receive file list\n");
	receive_file_list(transac);
      }
  if ((transac->header->grbg[1] == 0) && (transac->field))
    if ((transac->field->id == 213) ||
	(transac->field->id == 215) ||
	 (transac->field->id == 205) ||
	(transac->field->id == 201))
      {
	printf("receive file_info\n");
	receive_file_info(transac);
      }
/*   if ((transac->header->grbg[1] == 0) && (transac->field)) */
/*     if (((transac->field->id == 102) || (transac->field->id == 101)) */
/* 	&& (transac->nbr_param == 2)) */
/*       { */
/* 	printf("receive user_info\n"); */
/* 	receive_user_info(transac); */
/*       } */
  if ((transac->header->grbg[1] == 0) && (transac->field))
    if (((transac->field->id == 102) ||
	 (transac->field->id == 105) ||
	 (transac->field->id == 106) ||
	 (transac->field->id == 110)))
      {
	printf("receive get_user\n");
	receive_get_user(transac);
      }
  if ((transac->header->grbg[1] == 0) && (transac->field))
    if (transac->field->id == 101)
      {
	printf("receive user_login_list\n");
	receive_login_list(transac);
      }
}

void	launch_action(t_transac *transac, t_server *server)
{
  if (transac->header->grbg[1] == 354)
    {
      printf("receive User Access\n");
      receive_user_access(transac);
    }
  if (transac->header->grbg[1] == 301)
    {
      printf("receive Notify Change User\n");
      notify_change_user(transac);
    }
  if (transac->header->grbg[1] == 109)
    {
      printf("receive agreement\n");
      receive_agreement(transac);
      printf("server->auto_agree = %d\n", server->auto_agree);
      if (server->auto_agree == 1)
	agreed(gl_redcap->bridge->fd);
    }
  if (transac->header->grbg[1] == 106)
    {
      printf("receive chat\n");
      receive_chat(transac);
    }
  if (transac->header->grbg[1] == 104)
    {
      printf("receive privat message\n");
      receive_server_msg(transac);
    }
  if ((transac->header->grbg[1] == 355) && (transac->field))
    {
      printf("receive Broadcast\n");
      receive_broadcast(transac);
    }
  if (transac->header->grbg[1] == 100)
    {
      printf("receive erreur\n");
    }
  if (transac->header->grbg[1] == 302)
    {
      printf("receive notify delete user\n");
      receive_delete_user(transac);
    }
}

int	is_reply(t_transac     *transac)
{
  if (gl_redcap->debug->functions)
    printf("function:is_reply\n");
  if ((((u_int8_t *)(transac->header->grbg))[HDR_RPLY]) == 1)
    return (1);
  return (0);
}

int	which_action(t_transac     *transac)
{
  //fonction qui recherche l'action correspondant a ce reply
  return(9);
}

void	use_transac(t_transac     *transac, t_server *server)
{
  if (gl_redcap->debug->functions)
    printf("function:use_transac\n");
  if (gl_redcap->debug->transac)
    show_transac(transac);
  if (is_reply(transac))
    {
      which_action(transac);
      launch_reply(transac, server);
    }
  else
    launch_action(transac, server);
}

void	transac_field_free(t_field *field)
{
  ///
  ////
  /////
}

void	transac_free(t_transac     *transac)
{
	  free(transac->header);
	  if (transac->field)
	    {
	      free(transac->field->data);
	      if (transac->field->next)
		transac_field_free(transac->field);
	      free(transac->field);
	    }
	  free(transac);
}

