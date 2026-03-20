NAME    = BlueCap

SRCDIR  = src
OBJDIR  = build/obj
BINDIR  = build/bin

SRC     = main.cpp \
          MyFile.cpp CUser.cpp CDwlUpl.cpp \
          ClServ.cpp ClServ_Protocole.cpp ClServ_Tools.cpp \
          ClServ_MkTransactions.cpp ClServ_Action.cpp \
          ClServ_DwlUpl.cpp ClServ_CplxFields.cpp \
          Serv_Action.cpp Serv_Tools.cpp Serv.cpp \
          Transaction.cpp FlFileObj.cpp

SRCS    = $(addprefix $(SRCDIR)/, $(SRC))
OBJ     = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

LDFLAGS  = `pkg-config --libs Magick++`
CPPFLAGS = -I$(SRCDIR) -g -g3 -ggdb -Wall -ansi -pg `pkg-config --cflags Magick++`
CC       = g++

all: server

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(BINDIR)/$(NAME) *.core *~ *#

re: fclean all

server: $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJ) $(LDFLAGS) -o $(BINDIR)/$(NAME)
