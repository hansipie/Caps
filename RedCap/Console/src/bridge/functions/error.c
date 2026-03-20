#include "../../../includes/redcap.h"

void	resume_upload(t_thread_data *transfert)
{
  t_transac     *transac;
  t_data	*data;

  if(gl_redcap->debug->functions)
    printf("func:resume_upload\n");
  transac = malloc((sizeof(t_transac)) * MALLOC);
  data = malloc((sizeof(t_data)) * MALLOC);
  transac = build_hdr(transac, UPL_FILE, HDR_RQST);
  transac = build_field_s(transac, FILE_NAME, transfert->name);
  transac = build_field_i(transac, 204, 2);
  data = make_path(gl_redcap->engine->server->path, 1);
  free(transfert->status);
  transfert->status = strdup("resume");
  //    new_elem_task(pars->buffer, transac->header->id, UPL_FILE, strlen(pars->buffer) + pars->len + 130, 0);
  //  if (data)
  transac = build_field_b(transac, FILE_PATH, transfert->name, strlen(transfert->name));
  send_transac(transac);
/*   free(pars->buffer); */
/*   free(pars); */

}

void	receive_error(t_transac *transac)
{
  char	*buff;
  t_server		*server;

  if (gl_redcap->debug->functions)
    printf("function: receive_error\n");
  printf("error from id: %d\n", transac->header->id);
  server = gl_redcap->engine->server;
  while (server->transfert && (transac->header->id != server->transfert->id))
    server->transfert = server->transfert->next;
  if (server->transfert
      && (transac->header->id == server->transfert->id))
/*       && (server->transfert->up_down == UPL_FILE)) */
/*     resume_upload(server->transfert); */
/*   else */
    {
      free(server->transfert->status);
      server->transfert->status = strdup("error");
    }
  buff = malloc((transac->field->size + 2) * MALLOC);
  bcopy(transac->field->data, buff, transac->field->size);
  buff[transac->field->size] = 0;
  Writen(gl_redcap->bridge->fd, "\nerror: ");
  Writen(gl_redcap->bridge->fd, buff);
  Writen(gl_redcap->bridge->fd, "\n");
  free(buff);
  prompt(gl_redcap->bridge->fd);
}
