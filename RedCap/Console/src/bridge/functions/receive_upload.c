#include "../../../includes/redcap.h"

void	receive_upload(t_transac *transac)
{
  u_int32_t		rek;
  pthread_attr_t        attr;
  t_server		*server;
  u_int16_t             rek2;
  int                   toto;
  
  if (gl_redcap->debug->functions)
    printf("function:receive_upload\n");
  server = gl_redcap->engine->server;
  while ((transac->header->id != server->transfert->id) && server->transfert)
    server->transfert = server->transfert->next;
  if (!server->transfert)
    {
      printf("receive_download error receive strange download\n");
      return;
    }
  while ((transac->field) && (transac->field->id))
    {
      if (transac->field->size == 4)
	{
	  bcopy(transac->field->data, &rek, transac->field->size);
	  toto = ntohl(rek);
	}
      else
	{
	  bcopy(transac->field->data, &rek2, transac->field->size);
	  toto = ntohs(rek2);
	}
      if (transac->field->id == 107)
	{
	  server->transfert->reference = toto;
	}
      if (transac->field->id == 203)
	{
	  transac->field->data += 46;
	  server->transfert->resumage = toto;
	  printf("resumage:%d\n", server->transfert->resumage);
	}
      transac->field = transac->field->next;
    }  
  server->transfert->thread = malloc((sizeof(pthread_t)) * MALLOC);
  pthread_attr_init(&attr);
  if(pthread_create(server->transfert->thread, NULL, 
		    (void *)main_create_download, 
		    (t_thread_data *)server->transfert))
    printf("pthread_create error in open_connect in connect.c\n");
}
