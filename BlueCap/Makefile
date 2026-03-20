# the following library is needs on SUNs from my experience
# LIBS = -lxnet

NAME 	= BlueCap

SRC	= main.cpp \
	MyFile.cpp CUser.cpp CDwlUpl.cpp\
	ClServ.cpp ClServ_Protocole.cpp ClServ_Tools.cpp ClServ_MkTransactions.cpp ClServ_Action.cpp ClServ_DwlUpl.cpp ClServ_CplxFields.cpp \
	Serv_Action.cpp Serv_Tools.cpp Serv.cpp \
	Transaction.cpp FlFileObj.cpp 

OBJ	= $(SRC:.cpp=.o)

LDFLAGS	= `Magick++-config --ldflags --libs`

CPPFLAGS	= -I. -g  -g3 -ggdb -Wall -ansi -pg `Magick++-config --cppflags --cxxflags`

CC = g++

all: server

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) *.core *~ *#

re:	fclean all

server: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(NAME)

# DO NOT DELETE THIS LINE -- make depend depends on it.
