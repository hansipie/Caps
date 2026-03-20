#setenv LD_LIBRARY_PATH "/usr/pkg/lib"
NAME	= RedCap

SRC	= src/main.c src/init.c src/opts.c

SHARED	= src/shared/wrapsock.c src/shared/get_next_line.c src/shared/std.c \
	src/shared/read.c \
	src/shared/write.c \
	src/shared/buffer_list.c \
	src/shared/prompt.c \
	src/shared/print.c \
	src/shared/other.c \
	src/shared/non_blocking_fd.c	

ENGINE	= src/engine/main_engine.c \
	src/engine/functions/new_user.c \
	src/engine/functions/my_access.c \
	src/engine/functions/set_user_edit.c \
	src/engine/functions/pars_opt.c \
	src/engine/functions/get_user.c \
	src/engine/functions/delete_file.c \
	src/engine/functions/rename_file.c \
	src/engine/functions/user_info.c \
	src/engine/functions/get_file_info.c \
	src/engine/functions/lmv.c \
	src/engine/functions/help.c \
	src/engine/functions/server_name.c \
	src/engine/functions/lrm.c \
	src/engine/functions/mkdir.c \
	src/engine/functions/preview.c \
	src/engine/functions/task.c \
	src/engine/functions/connect.c \
	src/engine/functions/change_server.c \
	src/engine/functions/send_chat.c \
	src/engine/functions/send_priv_chat.c \
	src/engine/functions/login.c \
	src/engine/functions/me_info.c \
	src/engine/functions/get_user_list.c \
	src/engine/functions/get_bookmark.c \
	src/engine/functions/agreed.c \
	src/engine/functions/delete_user.c \
	src/engine/functions/get_file_list.c \
	src/engine/functions/disconnect.c \
	src/engine/functions/file_list_client.c \
	src/engine/functions/get_login_list.c \
	src/engine/functions/download.c \
	src/engine/functions/upload.c \
	src/engine/functions/manage_server.c \
	src/engine/functions/manage_task.c \
	src/engine/functions/send_broadcast.c \
	src/engine/functions/cd.c \
	src/engine/functions/pwd.c \
	src/engine/transaction.c \
	src/engine/transaction2.c \
	src/engine/flattened_file_object.c \
	src/engine/send_file.c \
	src/engine/manage_transac.c \
	src/engine/receive_file.c \
	src/engine/main_create_down-up.c

BRIDGE	= src/bridge/main_bridge.c \
	src/bridge/command.c \
	src/bridge/functions/receive_login_list.c \
	src/bridge/functions/receive_file_info.c \
	src/bridge/functions/receive_get_user.c \
	src/bridge/functions/receive_user_info.c \
	src/bridge/functions/receive_chat.c \
	src/bridge/functions/receive_broadcast.c \
	src/bridge/functions/receive_server_msg.c \
	src/bridge/functions/error.c \
	src/bridge/functions/receive_download.c \
	src/bridge/functions/receive_upload.c \
	src/bridge/functions/receive_agreement.c \
	src/bridge/functions/receive_user_list.c \
	src/bridge/functions/receive_user_access.c \
	src/bridge/functions/receive_file_list.c \
	src/bridge/functions/receive_login.c \
	src/bridge/functions/receive_delete_user.c 

#LIBS = -I/usr/include/  -L/usr/lib -lpthread # Mac OsX
LIBS = -I/usr/pkg/include -L/usr/pkg/lib -lpthread

INCL = -I.
CFLAGS = -g -g3 -ggdb -Wall
LDFLAGS = -g -lm
OBJS1 = ${SRC:.c=.o}
OBJS2 = $(ENGINE:.c=.o)
OBJS3 = $(BRIDGE:.c=.o)
OBJS4 = $(TCPSERV:.c=.o)
OBJS5 = $(SHARED:.c=.o)
CC = gcc
RM = rm -f

$(NAME): $(OBJS5) $(OBJS4) $(OBJS3) $(OBJS2) $(OBJS1)
	$(CC) -o $(NAME) $(LDFLAGS) $(LIBS) $(OBJS5) $(OBJS4) $(OBJS3) $(OBJS2) $(OBJS1)
	mv $(NAME) redcap_x/

clean:
	$(RM) $(OBJS5) $(OBJS4) $(OBJS3) $(OBJS2) $(OBJS1)
	$(RM) ./*~ ./src/*~ ./src/bridge/*~ ./src/bridge/functions/*~ ./src/engine/*~ ./src/engine/functions/*~ ./src/shared/*~ ./includes/*~

fclean: clean
	$(RM) rm redcap_x/RedCap

re: fclean $(NAME)