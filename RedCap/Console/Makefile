NAME    = RedCap

SRCDIR  = src
OBJDIR  = build/obj
BINDIR  = build/bin

SRC     = main.c \
          init.c \
          opts.c

SHARED  = shared/wrapsock.c \
          shared/get_next_line.c \
          shared/std.c \
          shared/read.c \
          shared/write.c \
          shared/buffer_list.c \
          shared/prompt.c \
          shared/print.c \
          shared/other.c

ENGINE  = engine/main_engine.c \
          engine/functions/new_user.c \
          engine/functions/my_access.c \
          engine/functions/set_user_edit.c \
          engine/functions/pars_opt.c \
          engine/functions/get_user.c \
          engine/functions/delete_file.c \
          engine/functions/rename_file.c \
          engine/functions/user_info.c \
          engine/functions/get_file_info.c \
          engine/functions/lmv.c \
          engine/functions/help.c \
          engine/functions/server_name.c \
          engine/functions/lrm.c \
          engine/functions/mkdir.c \
          engine/functions/preview.c \
          engine/functions/task.c \
          engine/functions/connect.c \
          engine/functions/change_server.c \
          engine/functions/send_chat.c \
          engine/functions/send_priv_chat.c \
          engine/functions/login.c \
          engine/functions/me_info.c \
          engine/functions/get_user_list.c \
          engine/functions/get_bookmark.c \
          engine/functions/agreed.c \
          engine/functions/delete_user.c \
          engine/functions/get_file_list.c \
          engine/functions/disconnect.c \
          engine/functions/file_list_client.c \
          engine/functions/get_login_list.c \
          engine/functions/download.c \
          engine/functions/upload.c \
          engine/functions/manage_server.c \
          engine/functions/manage_task.c \
          engine/functions/send_broadcast.c \
          engine/functions/cd.c \
          engine/functions/pwd.c \
          engine/transaction.c \
          engine/transaction2.c \
          engine/flattened_file_object.c \
          engine/send_file.c \
          engine/manage_transac.c \
          engine/receive_file.c \
          engine/main_create_down-up.c

BRIDGE  = bridge/main_bridge.c \
          bridge/command.c \
          bridge/functions/receive_login_list.c \
          bridge/functions/receive_file_info.c \
          bridge/functions/receive_get_user.c \
          bridge/functions/receive_user_info.c \
          bridge/functions/receive_chat.c \
          bridge/functions/receive_broadcast.c \
          bridge/functions/receive_server_msg.c \
          bridge/functions/error.c \
          bridge/functions/receive_download.c \
          bridge/functions/receive_upload.c \
          bridge/functions/receive_agreement.c \
          bridge/functions/receive_user_list.c \
          bridge/functions/receive_user_access.c \
          bridge/functions/receive_file_list.c \
          bridge/functions/receive_login.c \
          bridge/functions/receive_delete_user.c

ALL_SRC = $(SRC) $(SHARED) $(ENGINE) $(BRIDGE)
SRCS    = $(addprefix $(SRCDIR)/, $(ALL_SRC))
OBJS    = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

LIBS    = -lpthread
INCL    = -I.
CFLAGS  = -g -g3 -ggdb -Wall
LDFLAGS = -g -lm
CC      = gcc
RM      = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) $(LIBS) -o $(BINDIR)/$(NAME) $(OBJS) -lm

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCL) -c $< -o $@

clean:
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(BINDIR)/$(NAME)

re: fclean all

.PHONY: all clean fclean re
