#include "../../includes/redcap.h"


void free_transac(t_transac *transac)
{
  t_field *tmp, *tmp2;

  if (gl_redcap->debug->functions)
    printf("function:free_transac\n");
  free(transac->header);
  tmp = transac->field;
  while (tmp)
    {
      tmp2 = tmp->next;
      free(tmp->data);
      free(tmp);
      tmp = tmp2;
    }
  free(tmp);
  free(transac);
}

void		send_transac(t_transac *transac)
{
  t_data data;
/*   int i; */
  
  if (gl_redcap->debug->functions)
    printf("function:send_transac\n");
  if (gl_redcap->debug->transac)
    show_transac(transac);
  data = build_buff(transac);
/*   printf("-------------------------------------------------\n"); */
/*   printf("sending buffer of %d bytes:\n", data.len); */
/*   for (i = 0; i < data.len; i++) */
/*     printf("[%#x]", data.buffer[i]); */
/*   printf("\n-------------------------------------------------\n");   */
  xwrite(gl_redcap->engine->server->fd, data.buffer, data.len);
  free(data.buffer);
  free_transac(transac);
}

t_transac	*parse_transac(char *buff, int valsize)
{
  t_transac     *transac;
  t_hl_hdr      *header;
  t_field       *field;
  t_field	*ptr;
  char		*val;
  int		i;
 
  if (gl_redcap->debug->functions)
    printf("function:parse_transac\n");  
/*   printf("transaction dump header :"); */
/*   for (i = 0; i < 20; i++) */
/*     printf("[%#x]", buff[i]); */
/*   printf("\n"); */
  val = buff;
  transac = malloc((sizeof(t_transac)) * MALLOC);
  header = malloc((sizeof(t_hl_hdr)) * MALLOC);
  bcopy(val, &header->grbg[0], 2);
  val += 2;
  bcopy(val, &header->grbg[1], 2);
  header->grbg[1] = ntohs(header->grbg[1]);
  val += 2;
  bcopy(val, &header->id, 4);
  header->id = ntohl(header->id);
  val += 4;
  bcopy(val, &header->err, 4);
  header->err = ntohl(header->err);
  val += 4;
  bcopy(val, &header->total_len, 4); 
  header->total_len = ntohl(header->total_len);
  val += 4;
  bcopy(val, &header->data_len, 4);
  header->data_len = ntohl(header->data_len);
  val += 4;
  transac->header = header;
  bcopy(val, &transac->nbr_param, 2);
  transac->nbr_param = ntohs(transac->nbr_param);
  val += 2;
  transac->field = NULL;
  for (i = 0; i < transac->nbr_param; i++) 
    {
      field = malloc((sizeof(t_field)) * MALLOC);
      field->type = 0;
      bcopy(val, &field->id, 2);
      field->id = ntohs(field->id);
      val += 2;
      bcopy(val, &field->size, 2);
      field->size = ntohs(field->size);
      val += 2;
      field->data = malloc((field->size) * MALLOC);
      bcopy(val, field->data, field->size);
      val += field->size;
      field->next = NULL;
      ptr = transac->field;
      if (ptr == NULL)
	transac->field = field;
      else
	{
	  while (ptr->next)
	    ptr = ptr->next;
	  ptr->next = field;
	}
    }
/*   show_transac(transac); */
  //free(tmpbuff);
  return(transac);
} 


void show_transac(t_transac *transac)
{
  t_field *ptr;
  char *tmps;
  int *tmpi;
  int i;

  printf("-------------------------------------------------\n");       
  if (gl_redcap->debug->functions)
    printf("function:show_transac\n");
  printf("Flags: %d\n", ((u_int8_t *)(transac->header->grbg))[HDR_FLAG]);
  printf("Is reply: %d\n", ((u_int8_t *)(transac->header->grbg))[HDR_RPLY]);
  printf("Type: %d\n", transac->header->grbg[1]);
  printf("ID: %d\n", transac->header->id);
  printf("Error code: %d\n", transac->header->err);
  printf("Total size: %d\n", transac->header->total_len);
  printf("Data size: %d\n", transac->header->data_len);
  printf("Nbr of parameters: %d\n", transac->nbr_param); 
  ptr = transac->field; 
  while (ptr)
    {
      printf("Field ID: %d\n", ptr->id);
      printf("Field size: %d\n", ptr->size);
      switch (ptr->type)
	{
	case STRG:
	  tmps = (char *)ptr->data;
	  printf("Field data (string): ");
	  for (i = 0; i < ptr->size; i++)
	    printf("%c", (unsigned char)tmps[i]);
	  printf("\n");
	  break;
	case LINT:
	  tmpi = (int *)ptr->data;
	  printf("Field data (usigned long int): ");
	  printf("%d\n", ntohl(*tmpi));
	  break;
	case SINT:
	  tmpi = (int *)ptr->data;
	  printf("Field data (usigned short int): ");
	  printf("%d\n", ntohs(*tmpi));
	  break;
	case BIN:
	  printf("Field data (binary): ");
	  goto def;
	case 0:
	  printf("Field data: ");
	default:
	def:
	  tmps = (char *)ptr->data;
	  for (i = 0; i < ptr->size; i++)
	    printf("%#x ", tmps[i]);
	  printf("\n");
	}
      ptr = ptr->next;
    }
  printf("-------------------------------------------------\n");
/*   printf("engine fin show_transac:\n\tid %d\n\tconnected %d\n\tcaps_flag %d\n\tauto_agree %d\n\tversion %d\n\tserver_name %p\n\thost %p\n\tport %d\n\tlogin %p\n\tpasswd %p\n\tfd %d\n\tpath %p\n\tdata %p\n\tname %p\n\ticon %d\n\tthread %p\n\ttransfert %p\n\tnext %p\n\tprev %p\n", gl_redcap->engine->server->id, gl_redcap->engine->server->connected, gl_redcap->engine->server->caps_flag, gl_redcap->engine->server->auto_agree, gl_redcap->engine->server->version, gl_redcap->engine->server->server_name, gl_redcap->engine->server->host, gl_redcap->engine->server->port, gl_redcap->engine->server->login, gl_redcap->engine->server->passwd, gl_redcap->engine->server->fd, gl_redcap->engine->server->path, gl_redcap->engine->server->data, gl_redcap->engine->server->name, gl_redcap->engine->server->icon, gl_redcap->engine->server->thread, gl_redcap->engine->server->transfert, gl_redcap->engine->server->next, gl_redcap->engine->server->prev); */
}
