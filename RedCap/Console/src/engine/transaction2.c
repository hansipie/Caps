#include "../../includes/redcap.h"

t_transac *build_hdr(t_transac *transac, u_int16_t hdrid, u_int8_t is_reply)
{
  t_hl_hdr      *header;
  static int IdFlag = 1;

  if (gl_redcap->debug->functions)
    printf("function:build_hdr\n");
  header = malloc((sizeof(t_hl_hdr)) * MALLOC);
  ((u_int8_t *)(header->grbg))[HDR_FLAG] = HDR_FLAG;
  ((u_int8_t *)(header->grbg))[HDR_RPLY] = is_reply;
  header->grbg[1] = hdrid;
  header->id = IdFlag++;
  header->err = 0;
  header->total_len = 2;
  header->data_len = 2;
  transac->header = header;
  transac->field = NULL;
  transac->nbr_param = 0;
  return(transac);
}

t_transac *build_field(t_transac *transac, u_int16_t id, void *data, u_int16_t datasize, int type)
{
  t_field *field, *temp_field;
  

  if (gl_redcap->debug->functions)
    printf("function:build_field\n");
  field = malloc((sizeof(t_field)) * MALLOC);
  field->type = type;
  field->id = id;
  field->size = datasize;
  field->data = malloc((datasize) * MALLOC);
  bcopy(data, field->data, datasize);
  field->next = NULL;
  transac->header->data_len += datasize + 4;
  transac->header->total_len = transac->header->data_len;
  transac->nbr_param++;
  if (transac->field)
    {
      temp_field = transac->field;
      while (temp_field->next)
	temp_field = temp_field->next;
      temp_field->next = field;
    }
  else
    transac->field = field;
  return(transac);
}

t_transac *build_field_b(t_transac *transac, u_int16_t id, void *data, u_int16_t datasize)
{
  if (gl_redcap->debug->functions)
    printf("function:build_field_b\n");
  transac = build_field(transac, id, data, datasize, BIN);
  return(transac);
} 


t_transac *build_field_i(t_transac *transac, u_int16_t id, int data) 
{
  u_int32_t convert;

  if (gl_redcap->debug->functions)
    printf("function:build_field_i\n");
  if (data >= MAXUINT16)
    {
      convert = htonl(data);
      transac = build_field(transac, id, &convert, sizeof(u_int32_t), LINT);
    }
  else
    {
      convert = htons(data);
      transac = build_field(transac, id, &convert, sizeof(u_int16_t), SINT);
    }
  return(transac);
}

t_transac *build_field_s(t_transac *transac, u_int16_t id, char *data)
{
  if (gl_redcap->debug->functions)
    printf("function:build_field_s\n");      
  transac = build_field(transac, id, data, strlen(data), STRG);
  return(transac);
}


char btoc(char *bits)
{
  int mult, res, k, i;

  mult = 0;
  res = 0;
  k = 0;
  if (gl_redcap->debug->functions)
    printf("function:btoc\n");
  printf("\tbits:%s\n", bits);
  for (i = (int)strlen(bits) - 1; i >= 0; i--)
    {
      mult = (int)pow(2, (double)i);
      printf("\tmult:%d bits[%d]:%d\n", mult, k, bits[k]);
      if (bits[k] == '1')
	{
	  res+=mult;
	  printf("\t\tres:%d\n", res);
	}
      k++;
    }
  printf("\tretour:[%d]\n", res);
  return ((char)res);
}

char *bitsfield(char *field)
{
  int	iter, i;
  char	*bits, *tmp, *result;

  if (gl_redcap->debug->functions)
    printf("function:bitsfield\n");
  if ((strlen(field) % 8) != 0)
    {
      printf("error: BitsField with bad size\n");
      return (NULL);
    }
  result = malloc(9);
  iter = strlen(field) / 8;
  for (i = 0; i < iter; i++)
    {
      tmp = field;
      tmp += 8*i;
      bits = my_strndup(tmp, 8);
      result[i] = btoc(bits);
      free(bits);
    }
  return (result);
}

t_data build_buff(t_transac *transac)
{
  t_data data;
  char *tmp;
  u_int16_t tmp_short;
  u_int32_t tmp_long;
  t_field *ptr;

  if (gl_redcap->debug->functions)
    printf("function:build_buff\n");      
  data.len = (int)transac->header->data_len + 20;
  data.buffer = malloc((data.len) * MALLOC);
  tmp = data.buffer;
  bcopy(&transac->header->grbg[0], tmp, sizeof(u_int16_t));
  tmp += sizeof(u_int16_t);
  tmp_short = htons(transac->header->grbg[1]);
  bcopy(&tmp_short, tmp, sizeof(u_int16_t));
  tmp += sizeof(u_int16_t);
  tmp_long = htonl(transac->header->id);
  bcopy(&tmp_long, tmp, sizeof(u_int32_t));
  tmp += sizeof(u_int32_t);
  tmp_long = htonl(transac->header->err);
  bcopy(&tmp_long, tmp, sizeof(u_int32_t));
  tmp += sizeof(u_int32_t);
  tmp_long = htonl(transac->header->total_len);
  bcopy(&tmp_long, tmp, sizeof(u_int32_t));
  tmp += sizeof(u_int32_t);
  tmp_long = htonl(transac->header->data_len);
  bcopy(&tmp_long, tmp, sizeof(u_int32_t));
  tmp += sizeof(u_int32_t);
  tmp_short = htons(transac->nbr_param);
  bcopy(&tmp_short, tmp, sizeof(u_int16_t));
  tmp += sizeof(u_int16_t);
  ptr = transac->field;
  while (ptr)
    {
      tmp_short = htons(ptr->id);
      bcopy(&tmp_short, tmp, sizeof(u_int16_t));
      tmp += sizeof(u_int16_t);
      tmp_short = htons(ptr->size);
      bcopy(&tmp_short, tmp, sizeof(u_int16_t));
      tmp += sizeof(u_int16_t);
      bcopy(ptr->data, tmp, ptr->size);
      tmp += ptr->size;
      ptr = ptr->next;
    }
  return(data);
}
