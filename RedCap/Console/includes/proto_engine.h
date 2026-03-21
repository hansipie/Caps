void    my_access(int command);
void    get_login_list(int fd);
void    delete_file(int fd);
void    new_user(int fd);
void    set_user_edit(int fd);
void    get_user(int fd);
void	delete_user(int fd);
void    user_info(int fd);
void	my_mkdir(int fd);
void	transac_free(t_transac     *transac);
void	transac_field_free(t_field *field);
void	use_transac(t_transac     *transac, t_server *server);
void    cd_help(int connfd);
void    get_file_info(int connfd);
void    help_connect(int connfd);
void    help_set_user(int connfd);
void    help_send_broadcast(int connfd);
void    help_send_chat(int connfd);
void    help_send_priv_chat(int connfd);
void    help_task(int connfd);
void	free_opt(t_data *);
t_data  *pars_opt(char *, int, int);
void    lrm(int fd);
void    lrename(int fd);
void    rename_file(int fd);
void    lmkdir(int fd);
void    server_name(int connfd);
void		modify_task(char *buff, int id, int vers, int transfert_size, int resume);
void		stop_task(int fd);
void		delete_task(int fd);
void		cancel_task(int fd);
void	show_transfert(t_thread_data *transfert);
void    preview(int connfd);
void	   new_elem_task(char *buff, int id, int vers, int transfert_size, int resume);
void            insert_new_task(t_thread_data *new);
void	task();
void	show_task(t_thread_data *tmp);
void	send_file(t_thread_data *transfert, t_data *data);
void	receive_file(t_thread_data *transfert);
t_thread_data *fill_flattened_file_object(t_thread_data *transfert);
t_data    *send_flattened_file_object(t_thread_data *);
void	receive_flattened_file_object(t_thread_data *transfert);
t_transac	*parse_transac(char *, int);
void show_transac(t_transac *transac);
t_transac *build_field(t_transac *transac, u_int16_t id, void *data, u_int16_t datasize, int type);
void    main_create_download(t_thread_data *transfert);
void    send_cli_nfo(u_int16_t, int);
void    receive_file_list_header();
char	*convert(float t);
char	*calc_debit(long current_bytes, float cycle);
t_data  *make_path(char *, int);
void    main_file_list_client(int connfd);
t_data	*pars_download(char *buffer, int len);
void	receive_preview(t_thread_data *transfert);
void	download(int);
void	upload(int);
void	cd(int);
void	pwd(int);
void    get_file_list(int);
void    set_user(int);
void    reconnect(t_server *server, int);
void	me_info(int);
void	get_bookmark(int);
void	get_user_list(int);
void    notify_change_user(t_transac *);
void    disagreed(int);
void    agreed(int);
void    disconnect(int);
void	main_engine();
void	boucle_engine();

void    help(int connfd);


void	main_send_chat();
void	main_send_priv_chat();
void	main_send_broadcast();

t_server *manage_server(pthread_t *thread, t_server *server, char *manage);

void    previous_server();
void    next_server();

void            lst_server();
t_server	*list_server(pthread_t *thread, t_server *server);
t_server	*goto_server(pthread_t *thread, t_server *server);
t_server	*insert_server(pthread_t *thread, t_server *server);
t_server	*remove_server(pthread_t *thread, t_server *server);
t_server	*extract_server(pthread_t *thread, t_server *server);

void		main_connect(int);

int		str_cli(t_server *);

char		*add_field(char *buffer, u_int16_t id, u_int16_t sze, void *data, int *total_size);
/* t_hl_hdr        *build_hdr(u_int16_t hdrid, u_int8_t is_reply); */
t_transac	*build_hdr(t_transac *transac, u_int16_t hdrid, u_int8_t is_reply);
void            send_transac(t_transac *);

void		main_login(t_server *);

char btoc(char *bits);
char *bitsfield(char *field);
t_data		build_buff(t_transac *transac);
t_transac	*build_field_s(t_transac *transac, u_int16_t id, char *data);
t_transac	*build_field_i(t_transac *transac, u_int16_t id, int data);
t_transac	*build_field_b(t_transac *transac, u_int16_t id, void *data, u_int16_t datasize);
u_int32_t	locate_file(char *name);
