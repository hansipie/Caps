NAME     = caps

SRCDIR   = src
OBJDIR   = build/obj
BINDIR   = build/bin

CXX      = g++
CXXFLAGS = $(shell wx-config --cxxflags) -Wall \
           -I$(SRCDIR)/core \
           -I$(SRCDIR)/ui \
           -I$(SRCDIR)/chat \
           -I$(SRCDIR)/net \
           -I$(SRCDIR)/files \
           -I$(SRCDIR)/users
LDFLAGS  = $(shell wx-config --libs)

# Note: Adminstration.cpp (sans 'i') — typo conservée depuis l'original
SRC_CORE  = MyApp.cpp MyFrame.cpp MyPanel.cpp
SRC_UI    = Options.cpp PopupCreate.cpp ConfirmBox.cpp
SRC_CHAT  = Panel_PublicChat.cpp Panel_PrivChat.cpp ChatText.cpp \
            broadcast.cpp NewsGroup.cpp
SRC_NET   = connexion.cpp Connexion_client.cpp ServData.cpp engine.cpp
SRC_FILES = ExplorerServer.cpp ExplorerClient.cpp Download.cpp \
            Panel_Transfert.cpp preview.cpp FileList.cpp FileInfo.cpp \
            Rep.cpp
SRC_USERS = User.cpp Utilisateur.cpp Adminstration.cpp \
            create_account.cpp Agree.cpp crypto.cpp

SRCS = $(addprefix $(SRCDIR)/core/,  $(SRC_CORE))  \
       $(addprefix $(SRCDIR)/ui/,    $(SRC_UI))     \
       $(addprefix $(SRCDIR)/chat/,  $(SRC_CHAT))   \
       $(addprefix $(SRCDIR)/net/,   $(SRC_NET))    \
       $(addprefix $(SRCDIR)/files/, $(SRC_FILES))  \
       $(addprefix $(SRCDIR)/users/, $(SRC_USERS))

OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

all: $(BINDIR)/$(NAME)

$(BINDIR)/$(NAME): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(BINDIR)/$(NAME)

re: fclean all

.PHONY: all clean fclean re
