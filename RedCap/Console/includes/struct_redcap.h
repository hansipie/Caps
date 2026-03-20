
/* */
/* *** structure RedCap *** */



typedef struct	    s_fork_info_list
{
  char		    *fork;
  u_int32_t	    data_size;
  char		    *rsvd_a;
  char		    *rsvd_b;
}		    t_fork_info_list;

typedef struct      s_flat_file
{
  char		    *format;
  u_int16_t	    version;
  char		    *rsvd;
  u_int16_t	    fork_count;
  t_fork_info_list  *fork_info_list;
}		    t_flat_file;

typedef struct      s_flat_file_info
{
  char		    *fork_type;
  u_int32_t	    compression_type;
  char		    *rsvd;
  u_int32_t	    data_size;
}		    t_flat_file_info;

typedef struct      s_flat_file_info_fork
{
  char		    *plateform;
  char		    *type_signature;
  char		    *creator_signature;
  u_int32_t	    flags;
  u_int32_t	    plateform_flags;
  char		    *rsvd;
  char		    *create_date;
  char		    *modify_date;
  u_int16_t	    name_script;
  u_int16_t	    name_size;
  char		    *name;
}		    t_flat_file_info_fork;

/* structure de trasfert */

typedef struct		s_thread_data
{
	char			  *name;
	char			  *file_path;
	u_int32_t		  transfert_size;
	u_int32_t		  file_size;
	u_int32_t		  current_size;
	u_int32_t	  file_transfert_option; // resume
	int			reference;
	u_int32_t	     	waiting_count;
	char		       	*status;
	int		       	resume;
	  int		       	id;
	int		       	resumage;
	int		       	fd;
	int		       	port;
	char		       	*flow; // debit
	pthread_t	       	*thread;
	t_flat_file		*header;
	t_flat_file_info	*info_fork;
	t_flat_file_info_fork	*info;
	t_flat_file_info	*fork;
	int				up_down;
	struct s_thread_data		*next;
}							t_thread_data;

/* structure serveur */

typedef struct		s_server
{
  int			auto_reconnect;
	int			id;		/* local server id */
	char			my_access[8];
	int			connected;	/* connected or not */ 
	int			caps_flag;	/* connected on a caps server or not */
	int			auto_agree;	/* auto accept aggrement or not */
	int			version;	/* version use by the server */
	char			*server_name;	/* nom du serveur courant  */
	char			*host;		/* adresse of serveur */
	int			port;		/* port of server */
	char			*login;		/* login use to connect on server */
	char			*passwd;	/* password use to connect on server */
	int			fd;		/* fd of the connected server */
	char			*path;
	t_data			*data;		/* data receive from serveur  */
	char			*name;
	int			icon;
	/*   t_data		*wait_transac; */
	pthread_t		*thread;	/* thread of the server */
	t_thread_data		*transfert;
	pthread_mutex_t		mutex;		/* mutex of the server*/
	pthread_cond_t		cond;
	struct s_server		*next;		/* next serveur */
	struct s_server		*prev;		/* previous serveur */
}					t_server;

typedef struct      s_manage_server
{
	char            *manage;
	t_server        *(*manage_fct)();
}                   t_manage_server;

/* structure specifique a l'engine */

typedef struct		s_engine
{
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	t_server		*server;
	/*   t_thread_data		*thread_data; */
	t_function		*functions_client;
	t_function		*functions_server;
}					t_engine;

/* structure specifique au bridge */

typedef struct		s_bridge
{
	int			port;
	char			*passwd;
	int			fd;
	int			nbr;
	t_data			*data;		/* donnees recu de IHM  */
	t_function		functions_client;
}			t_bridge;

typedef struct          s_debug
{
	int			other;
	int			read;
	int			write;
	int			thread;
	int			functions;
	int			transac;
}			t_debug;

/* structure globale de Redcap */

typedef struct		s_redcap
{
	char			*name;
	int			icon;
	t_debug			*debug;
	t_engine		*engine;
	t_bridge		*bridge;
}			t_redcap;

typedef struct		s_opt	
{
	int			connected;
	int			caps_flag;
	int			auto_agree;
	char			*host;
	char			*port;
	char			*login;
	char			*passwd;
	int			connfd;
	pthread_t             *thread;  
}			t_opt;
