bin_PROGRAMS = redcap
redcap_SOURCES = main.c init.c opts.c \
		shared/wrapsock.c \
		shared/get_next_line.c \
		shared/std.c \
		shared/read.c \
		shared/write.c \
		shared/prompt.c \
		shared/buffer_list.c \
		shared/print.c \
		shared/other.c \
		engine/main_engine.c \
		engine/functions/help.c \
		engine/functions/connect.c \
		engine/functions/task.c \
		engine/functions/change_server.c \
		engine/functions/send_chat.c \
		engine/functions/send_priv_chat.c \
		engine/functions/login.c \
		engine/functions/me_info.c \
		engine/functions/get_user_list.c \
		engine/functions/get_bookmark.c \
		engine/functions/agreed.c \
		engine/functions/get_file_list.c \
		engine/functions/disconnect.c \
		engine/functions/file_list_client.c \
		engine/functions/download.c \
		engine/functions/upload.c \
		engine/functions/manage_server.c \
		engine/functions/send_broadcast.c \
		engine/functions/cd.c \
		engine/functions/pwd.c \
		engine/transaction.c \
		engine/transaction2.c \
		engine/flattened_file_object.c \
		engine/send_file.c \
		engine/receive_file.c \
		engine/main_create_down-up.c \
		bridge/main_bridge.c \
		bridge/command.c \
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

LIBS = -I/usr/pkg/include -L/usr/pkg/lib -lpthread
		
# set the include path found by configure
INCLUDES= $(all_includes)

# the library search path.
redcap_LDFLAGS = $(all_libraries) 
