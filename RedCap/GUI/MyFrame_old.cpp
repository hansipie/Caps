#include "general.h"

MyFrame::MyFrame(const wxChar *title, int x, int y)
       : wxFrame(NULL, -1, title, wxPoint(x, y), wxSize(800, 600))
{
//	wxIcon *icon;
	//wxIcon *icon = new wxIcon("aiai");
	//icon = icon->LoadFile("./icons/Redcap.ico", wxBITMAP_TYPE_ICO);
//	SetIcon(icon->LoadFile("./icons/Redcap.ico", wxBITMAP_TYPE_ICO));

    //SetIcon( wxICON("./icons/Redcap.ico") );
	CreateMyMenuBar();
	CreateMyToolBar();
	m_panel = new MyPanel( this, 10, 10, 300, 100 );
	CreateStatusBar(2);
	SetSizeHints( 500, 425 );
	Name = "RedCapUser";
	Icone = "314";
	//ca sert au client console uniquement

	//Client_Host = "localhost";
	Client_Host = "192.168.255.128";

	Client_Port = "5505";
	//dernieres variables enregistrees par la fenettre de connection au serveur 
	Serv_User = "guest"; 
	Serv_Passwd = "";
	//Serv_Host = "192.168.254.1";
	Serv_Host = "10.251.152.67";
	//Serv_Host = "10.251.152.67";
	//Serv_Host = "192.168.163.1";
	Serv_Port = "";
	//est on connecte a la console ?
	is_connected_console = 0;

	//options
	mess_join_leave_chat = TRUE;
	accept_priv_mess = TRUE;
	accept_priv_chat = TRUE;
	automatic_response = FALSE;
	log_mess = TRUE;
	auto_agree = FALSE;
}


MyFrame::~MyFrame()
{	
  // No delayed deletion here, as the frame is dying anyway
//  delete m_sock;
}


void MyFrame::auto_launch()
{
	Connexion_Client dialog_client(this, -1, wxT("Blue CAPS client connection"), wxDefaultPosition);
	dialog_client.CentreOnParent();
    if (dialog_client.ShowModal() == dialog_client.BUTTON_CONNECT_CLIENT)
	{
		wxIPV4address addr1;
		m_sock = new wxSocketClient();
		
		m_sock->SetEventHandler(*this, SOCKET_ID);
		m_sock->SetNotify(wxSOCKET_CONNECTION_FLAG |
						wxSOCKET_INPUT_FLAG |
						wxSOCKET_LOST_FLAG);
		m_sock->Notify(TRUE);
		m_busy = FALSE;
		addr1.Hostname(Client_Host);
		addr1.Service(Client_Port);
		//	m_panel->m_text->AppendText(_("\nTrying to connect Consol RedCap Client(timeout = 10 sec) ...\n"));
		m_sock->Connect(addr1, FALSE);
		m_sock->WaitOnConnect(10);
		
		if (m_sock->IsConnected())
		{
			is_connected_console = 1;
			connect_menu->Enable(CONTROLS_CONNECT, TRUE);
			connect_menu->Enable(CONTROLS_CONNECT_CLIENT, FALSE);

			StatusText = "Connected to client, now connect a server please...";
			//demande le login, l'icone de l'utilisateur et la liste des bookmarks
			//output_message("me_info");
			//output_message("bookmark_get_list");
		}
		else
		{
			m_sock->Close();
		}
	}
	return ;
}

void MyFrame::CreateMyMenuBar()
{
	file_menu = new wxMenu;
	connect_menu = new wxMenu;
	help_menu = new wxMenu;

    file_menu->Append(CONTROLS_CLEAR_LOG, _T("&Clear log\tCtrl-L"), _T("Clear log"));
    file_menu->Append(CONTROLS_OPTIONS, _T("&Options"), _T("Options"));
    file_menu->AppendSeparator();
    file_menu->Append(CONTROLS_QUIT, _T("E&xit\tAlt-X"), _T("Quit controls sample"));
	
	connect_menu->Append(CONTROLS_CONNECT, _T("Connect to server\tCtrl-S"), _T("Connect..."));
	connect_menu->Enable(CONTROLS_CONNECT, FALSE ); // sera mis a "enable=true" si conection au client

	connect_menu->Append(CONTROLS_DISCONNECT, _T("Close connection\tCtrl-D"), _T("Disconnect..."));
	connect_menu->AppendSeparator();
	connect_menu->Append(CONTROLS_CONNECT_CLIENT, _T("Connect to Consol Client\tCtrl-G"), _T("Connect..."));

	help_menu->Append(CONTROLS_HELP, _T("&Help\tF1"), _T("Help CAPS"));
	help_menu->AppendSeparator();
	help_menu->Append(CONTROLS_ABOUT, _T("&About\tF1"), _T("About CAPS"));

	wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(file_menu, _T("&File"));
	menu_bar->Append(connect_menu, _T("&Connexion"));
	menu_bar->Append(help_menu, _T("&?"));
	SetMenuBar( menu_bar );
}

void MyFrame::CreateMyToolBar()
{
	wxToolBar *tb = CreateToolBar( wxTB_HORIZONTAL|wxNO_BORDER  | wxTB_FLAT);
	
	
	int x = 0, y = 0;
	GetClientSize(&x, &y);
	bitmap_title = new wxStaticBitmap(tb, -1,
									wxBitmap("./icons/titre.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(x - 520 ,0),
									wxDefaultSize
									);
	degrade_title = new wxStaticBitmap(tb, -1,
									wxBitmap("./icons/degrade_titre.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(x - 290 , 0),
									wxDefaultSize
									);
	wxBitmap toolBarBitmaps[6];
	toolBarBitmaps[0] = wxBitmap("./icons/connect.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[1] = wxBitmap("./icons/about.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[2] = wxBitmap("./icons/options.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[3] = wxBitmap("./icons/send_broadcast.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[4] = wxBitmap("./icons/client_connect.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[5] = wxBitmap("./icons/disconnect.bmp",wxBITMAP_TYPE_BMP);

	wxMask *mask0 = new wxMask(toolBarBitmaps[0], *wxWHITE);
    toolBarBitmaps[0].SetMask(mask0);
	wxMask *mask1 = new wxMask(toolBarBitmaps[1], *wxWHITE);
    toolBarBitmaps[1].SetMask(mask1);
	wxMask *mask2 = new wxMask(toolBarBitmaps[2], *wxWHITE);
    toolBarBitmaps[2].SetMask(mask2);
	wxMask *mask3 = new wxMask(toolBarBitmaps[3], *wxWHITE);
    toolBarBitmaps[3].SetMask(mask3);
	wxMask *mask4 = new wxMask(toolBarBitmaps[4], *wxWHITE);
    toolBarBitmaps[4].SetMask(mask4);
	wxMask *mask5 = new wxMask(toolBarBitmaps[5], *wxWHITE);
    toolBarBitmaps[5].SetMask(mask5);

	tb->SetToolBitmapSize(wxSize(toolBarBitmaps[3].GetWidth(),toolBarBitmaps[3].GetHeight()));
	tb->AddTool (CONTROLS_CONNECT_CLIENT, "Connect to RedCap Client Consol", toolBarBitmaps[4], _T("Connect to RedCap Client Consol"));
	tb->AddTool (CONTROLS_CONNECT, "Connect to a server", toolBarBitmaps[0], _T("Connect to a server"));
	tb->AddTool (CONTROLS_DISCONNECT, "Disconnection", toolBarBitmaps[5], _T("Disconnect the Server"));
	tb->AddTool (CONTROLS_OPTIONS, "Configuration", toolBarBitmaps[2], _T("Configuration"));
	tb->AddTool (CONTROLS_BROADCAST, "Send a Broadcast", toolBarBitmaps[3], _T("Send Broadcast"));
	tb->AddTool(CONTROLS_ABOUT, "About RedCap", toolBarBitmaps[1], _T("About RedCap"));
	tb->Realize();
}

void MyFrame::OnClearLog(wxCommandEvent& WXUNUSED(event))
{
;//    m_panel->m_text->Clear();
}


void MyFrame::OnMove( wxMoveEvent& event )
{

    UpdateStatusBar(event.GetPosition(), GetSize());

    event.Skip();
}

void MyFrame::OnSize( wxSizeEvent& event )
{
    UpdateStatusBar(GetPosition(), event.GetSize());

    event.Skip();
}

void MyFrame::OnIdle( wxIdleEvent& WXUNUSED(event) )
{
    // track the window which has the focus in the status bar
/*    static wxWindow *s_windowFocus = (wxWindow *)NULL;
    wxWindow *focus = wxWindow::FindFocus();
    if ( focus && (focus != s_windowFocus) )
    {
        s_windowFocus = focus;

        wxString msg;
        msg.Printf(
#ifdef __WXMSW__
                _T("Focus: %s, HWND = %08x"),
#else
                _T("Focus: %s"),
#endif
                s_windowFocus->GetClassInfo()->GetClassName()
#ifdef __WXMSW__
                , (unsigned int) s_windowFocus->GetHWND()
#endif
                  );
    }*/
//  if (StatusText && StatusText != NULL)
  //  SetStatusText(StatusText);
} 

void MyFrame::OnConnect( wxCommandEvent &event )
{
	//Pour la connection
	//"connect login[:password]@host[:port]"
	wxIPV4address addr1;
	wxString s;

    Connexion dialog(this, -1, wxT("Server RedCAP Connexion"), wxDefaultPosition);
	dialog.CentreOnParent();
	if (dialog.ShowModal() == dialog.BUTTON_CONNECT)
	{
		if (Serv_User != "" && Serv_Host != "")
		{
			s = "connect " + Serv_User;
			if (Serv_Passwd!= "")
				s = s + ":" + Serv_Passwd + "@" + Serv_Host;
			if (Serv_Passwd == "" && Serv_Host  != "")
				s = s + "@" + Serv_Host;
			if (Serv_Port != "")
				s = s + ":" + Serv_Port;
			if (connect_opt_ag)
				s = s + " auto_agree";
		output_message(s);
		}
		else
			wxMessageBox("Unable to connect ! Host and login values are needed");
	}
}

void MyFrame::OnConnect_client( wxCommandEvent &event)
{
	Connexion_Client dialog_client(this, -1, wxT("Blue CAPS client connection"), wxDefaultPosition);
	dialog_client.CentreOnParent();
    if (dialog_client.ShowModal() == dialog_client.BUTTON_CONNECT_CLIENT)
	{
		wxIPV4address addr1;
		m_sock = new wxSocketClient();
		
		m_sock->SetEventHandler(*this, SOCKET_ID);
		m_sock->SetNotify(wxSOCKET_CONNECTION_FLAG |
						wxSOCKET_INPUT_FLAG |
						wxSOCKET_LOST_FLAG);
		m_sock->Notify(TRUE);
		m_busy = FALSE;
		addr1.Hostname(Client_Host);
		addr1.Service(Client_Port);
		//	m_panel->m_text->AppendText(_("\nTrying to connect Consol RedCap Client(timeout = 10 sec) ...\n"));
		m_sock->Connect(addr1, FALSE);
		m_sock->WaitOnConnect(10);
		
		if (m_sock->IsConnected())
		{
			is_connected_console = 1;
			connect_menu->Enable(CONTROLS_CONNECT, TRUE);
			connect_menu->Enable(CONTROLS_CONNECT_CLIENT, FALSE);

			StatusText = "Connected to client, now connect a server please...";
			//demande le login, l'icone de l'utilisateur et la liste des bookmarks
			//output_message("me_info");
			//output_message("bookmark_get_list");
		}
		else
		{
			m_sock->Close();
		}
	}
}

void MyFrame::OnAbout( wxCommandEvent& WXUNUSED(event) )
{
	wxMessageDialog dialog(this, _T("Made By :\n- Nicolas Baret\n- Christopher Premaillon\n- Gregory Koeff\n - Hansi Pierre\n- Michel Le Cocq\n\nwww.caps-project.com\n\nEpitech 2004"), _T("CAPS Project V1.0"), wxOK | wxICON_INFORMATION );
    dialog.ShowModal();
}
void MyFrame::OnSendBroadcast( wxCommandEvent& WXUNUSED(event) )
{
	BroadcastFlag = 2;
	BroadcastDialog broadcast_box(this, -1, wxT(".:: BROADCAST Message ::."), wxDefaultPosition);
	broadcast_box.CentreOnParent();
	broadcast_box.ShowModal();  
}


void MyFrame::OnQuit (wxCommandEvent& WXUNUSED(event) )
{
    Close(TRUE);
}

void MyFrame::OnSocketEvent(wxSocketEvent& event)
{
  wxString s = _("OnSocketEvent: ");

  switch(event.GetSocketEvent())
  {
	case wxSOCKET_INPUT      : s.Append(_("Input text\n"));input_message(event); break;
    case wxSOCKET_LOST       : s.Append(_("Console client lost\n")); lost_console(); break;
    case wxSOCKET_CONNECTION : s.Append(_("Connecting\n")); break;
    default                  : s.Append(_("Unexpected event !\n")); break;
  }

  //m_panel->m_text->AppendText(s);
}
void MyFrame::auto_agreement()
{
	m_panel->list_users->DeleteAllItems();
	m_panel->panel_transfer->ExploServer->tree_transfer->DeleteAllItems();
	m_panel->panel_transfer->ExploServer->tree_list->DeleteAllItems();
	m_panel->panel_transfer->ExploClient->tree_transfer->DeleteAllItems();
	m_panel->panel_transfer->ExploClient->tree_list->DeleteAllItems();
	

	output_message("get_file_list");
	output_message("get_client_file_list");
}

void MyFrame::input_message(wxSocketEvent& event)
{
	vector<wxString> tab;
	char *tmpbuf;
	wxString stmpbuf;
	//int flag;

	m_busy = TRUE;
	

    if (m_sock->IsData())
      {
				tmpbuf = new char[4096];
				m_sock->Read(tmpbuf, 4096);
				tmpbuf[m_sock->LastCount()] = '\0';

				stmpbuf = wxString(tmpbuf);

				//wxMessageBox(" texte lu sur la socket    " + stmpbuf);

				buf = buf + tmpbuf;

				//wxMessageBox("--" + stmpbuf.substr(stmpbuf.length() - 4, 3)  + "--" );

				if (!wxStrcmp(stmpbuf.substr(stmpbuf.length() - 4, 3), "] %"))
				{
					//wxMessageBox("commande " + buf);
					tab_line_cmd = StringToTab(wxString(buf), "\n");
					for ( vector<wxString>::iterator i = tab_line_cmd.begin(); i != tab_line_cmd.end(); ++i )
						{
							line_cmd = *i;
							tab_arg_line_cmd.clear();
							tab_arg_line_cmd = StringToTab(line_cmd, " ");
	
							if (tab_arg_line_cmd.size()> 0)
							{
								wxString tmp = tab_arg_line_cmd.at(0);
								//wxMessageBox("premire mot   " + tmp);
								if (tab_arg_line_cmd.size() > 2 &&  !wxStrcmp(tmp, ":") && !wxStrcmp(tab_arg_line_cmd.at(1), "Welcome"))
									wxMessageBox("You are connected to RedCap Console");
								if (tab_arg_line_cmd.size() > 2 &&  !wxStrcmp(tmp, "notify") && !wxStrcmp(tab_arg_line_cmd.at(1), "change") && !wxStrcmp(tab_arg_line_cmd.at(2), "user:"))
									user_change(i);
								if (!wxStrcmp(tmp, "relogin"))
									relogin();
								if (!wxStrcmp(tmp, "connected"))
									connect_ok(i);
								if (!wxStrcmp(tmp, "Agreement:") && !connect_opt_ag)
									agree();
								if (!wxStrcmp(tmp, "Agreement:") && connect_opt_ag) 
									auto_agreement();
								if (!wxStrcmp(tmp, "chat_msg:"))
									chat_msg(i);
								if (!wxStrcmp(tmp, "user_list:"))
									user_set_list(i);
								if (!wxStrcmp(tmp, "set_files_list:"))
									file_set_list(i);
								if (!wxStrcmp(tmp, "set_client_files_list:"))			
									file_client_set_list(i);
 								if (!wxStrcmp(tmp, "receive_priv_chat"))
									receive_priv_chat(i);
								if (!wxStrcmp(tmp, "broadcast_msg"))
									broadcast_msg(i);
								if (!wxStrcmp(tmp, "receive_server_msg"))
									receive_server_msg(i);

							}
						}
					buf.clear();
				}
		}
	m_busy = FALSE;
	delete tmpbuf;
}

void MyFrame::output_message(wxString str)
{
	char a;
	int len;
	wxString msg;

	msg = wxStrdup(str);

	if (is_connected_console == 0)
		return;

//	m_panel->m_text->AppendText(msg);

	a = 13;
	msg = msg + a;
	a = 10;
	msg = msg + a;
	a = 0;
	msg = msg + a;
    
	//wxMessageBox("j'envoie au console : -" + msg + "--");
	len  = (wxStrlen(msg)) * sizeof(wxChar);
	m_busy = TRUE;
	m_sock->Write(msg, len);
	m_busy = FALSE;
}



void MyFrame::OnOptions(wxCommandEvent &event)
{
    COptions  dialog(this, -1, wxT("RedCAP options"), wxDefaultPosition);
	dialog.CentreOnParent();
	dialog.ShowModal();
}


vector<wxString> MyFrame::StringToTab(wxString chaine, wxString sep)
{
	vector<wxString> tab;
	unsigned int	a;
	unsigned int b;
	unsigned int c;

	a = 0;
	tab.clear();

	for  (c = a; c < chaine.length() && !wxStrncmp(  chaine.substr(c, chaine.length() - c) ,  sep, sep.length())         ; c++)
		;
	if (c == chaine.length())
		return tab;
	a = c;
	while (a < chaine.length())
	{
		b = chaine.find_first_of(sep, a);
//		wxMessageBox("--" + chaine.substr(a, b - a) + "---" );
		tab.push_back(chaine.substr(a, b - a));
//		for  (c = b; c < chaine.length() && chaine.at(c) == ' '; c++)
		for  (c = b; c < chaine.length() &&   !wxStrncmp(chaine.substr(c, chaine.length() - c) ,  sep, sep.length())         ; c++)
			;
		a = c;
	}
	return tab;
}
