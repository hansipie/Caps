

void  list_data(t_data *);
char	*copy_buff(char *transac, int size);
void    hl_code (void *__dst, const void *__src, size_t len);
int     xwrite(int fd, void *buff, int len);

char    *get_next_line(int fd);
int     is_host(char *str);
char    *my_strcat(char *s1, char *s2);
/* void    get_nbr(int n, char *s); */
char	*my_strndup(char *str, int len);
void    str_to_tab_clean(char **tab);
char	**str_to_tab(char *str, int len, char sep);
void    show_data_list(t_data *);

void	prompt(int);

/* Defines */
/* defined before the right type get a chance to be defined */
#define my_socklen_t unsigned int
/* maximum number of pending connections */
#define LISTENQ 128
/* convenience to make it easier to read the code */
#define SA struct sockaddr
/* length of a line */
#define MAXLINE 4096
#define MALLOC 1

/* Typedefs */
/* Convenient definition to make it easier to read the code */
typedef void Sigfunc(int);

/* Functions protocols */

/* Used by server */
void servusage(char *);
void Bind(int, const struct sockaddr *, my_socklen_t);
void Listen(int, int);
Sigfunc *Signal(int, Sigfunc *);
pid_t Fork(void);
void Close(int);
t_data *Read_from_server(int, t_data *, t_server *);
t_data *Read_from_bridge(int, t_data *);

void	my_putstr(char *);

t_data  *insert_data(t_data *buffer, t_data *data);
t_data  *remove_data(t_data *data);

/* Used by client and server */
int Socket(int, int, int);

/* Used to read a line */
char * Fgets(char *, int, FILE *);
/* ssize_t mg_readline(int, void *, size_t); */
/* ssize_t Readline(int, void *, size_t); */

/* Used to write a line */
void Fputs(const char *, FILE *);
ssize_t writen(int, const void *, size_t);
/* void Writen(int, void *, size_t); */
void Writen(int, char *);

/* Used to handle SIGCHLD */
Sigfunc * mg_signal(int, Sigfunc *);

/* Used to wait on requests */
int Select(int, fd_set *, fd_set *, fd_set *, struct timeval *);

/* Used to close the socket in  child process */
void Shutdown(int, int);

/* Used to handle errors */
void err_sys(const char *fmt, ...);
void err_quit(const char *fmt, ...);

void set_non_blocking ( int m_sock, int b);
