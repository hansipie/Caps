CXX      = g++
CXXFLAGS = $(shell wx-config --cxxflags) -Wall
LDFLAGS  = $(shell wx-config --libs)
PROGRAM  = caps

SRCS = Adminstration.cpp broadcast.cpp Agree.cpp ChatText.cpp \
       Connexion_client.cpp connexion.cpp create_account.cpp crypto.cpp \
       Download.cpp engine.cpp ExplorerClient.cpp ExplorerServer.cpp \
       FileInfo.cpp FileList.cpp MyApp.cpp MyFrame.cpp MyPanel.cpp \
       NewsGroup.cpp Options.cpp Panel_PrivChat.cpp Panel_PublicChat.cpp \
       Panel_Transfert.cpp PopupCreate.cpp preview.cpp Rep.cpp \
       ServData.cpp User.cpp Utilisateur.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(PROGRAM)

re: fclean all

.PHONY: all clean fclean re
