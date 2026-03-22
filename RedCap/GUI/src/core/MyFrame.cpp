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
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(m_panel, 1, wxEXPAND);
	SetSizer(mainSizer);
	CreateStatusBar(2);
	int size[2];
	size[0] = 20;
	size[1] = -1;
	SetStatusWidths(2, size);

	SetSizeHints( 500, 425 );
	Layout();
	Name = "RedCapUser";
	Icone = "314";

	


	//ca sert au client console uniquement
	//Client_Host = "192.168.169.128"; // Ecole Greg
	//Client_Host = "192.168.1.11";//soutenance
	Client_Host = "192.168.75.130";
//    Client_Host = "192.168.142.128"; // Ip Maison greg
	Client_Port = "5505";
	
	//dernieres variables enregistrees par la fenettre de connection au serveur 
	//Serv_User = "admin"; 
	//Serv_Passwd = "passadmin";
	Serv_User = "guest"; 
	Serv_Passwd = "";
	//Serv_Host = "10.248.2.13"; // Ecole Greg
	//Serv_Host = "192.168.1.3";// soutenance
	Serv_Host = "192.168.75.1";
//	Serv_Host = "192.168.163.1";
//	Serv_Host = "192.168.142.1"; // Maison Greg
	Serv_Port = "";
	is_connected_console = 0;

	//options
	mess_join_leave_chat = TRUE;
	accept_priv_mess = TRUE;
	accept_priv_chat = TRUE;
	automatic_response = FALSE;
	log_mess = TRUE;
	auto_agree = FALSE;
	opt_crypto = FALSE;
	Pass_crypto = "RedCap Password";

	if ( m_frameStatusBar )
    {
		SetStatusText("Connection : not connected.", 1);
	}
	buf = "";

	file_menu->Enable(CONTROLS_OPTIONS, FALSE);
	file_menu->Enable(CONTROLS_ADMIN, TRUE);
	connect_menu->Enable(CONTROLS_CONNECT, FALSE);
	connect_menu->Enable(CONTROLS_DISCONNECT, FALSE);
	connect_menu->Enable(CONTROLS_CONNECT_CLIENT, TRUE);
	m_panel->Enable(true);
	toolbar->EnableTool(CONTROLS_CONNECT_CLIENT, true);
	toolbar->EnableTool(CONTROLS_CONNECT, false);
	toolbar->EnableTool(CONTROLS_DISCONNECT, false);
	toolbar->EnableTool(CONTROLS_OPTIONS, false);
	toolbar->EnableTool(CONTROLS_BROADCAST, false);
	toolbar->EnableTool(CONTROLS_ADMIN, false);
	m_panel->panel_public->SendChatButton->Enable(false);
	m_panel->panel_transfer->ExploClient->download_button->Enable(false);
	m_panel->panel_transfer->ExploClient->upload_button->Enable(false);
	m_panel->panel_transfer->ExploServer->delete_button->Enable(false);
	m_panel->panel_transfer->ExploServer->create_folder_button->Enable(false);
	m_panel->panel_transfer->ExploServer->preview_button->Enable(false);

	preview_size = 0;
	preview_curr_size = 0;
	tmpbuf = new char[4096];
	outputcmd.clear();
	connexion_name = "";
	connexion_capsproto = false;
	login_in_progress = false;
}
MyFrame::~MyFrame()
{
  // No delayed deletion here, as the frame is dying anyway
//  delete m_sock;
}
void MyFrame::CreateMyMenuBar()
{
	file_menu = new wxMenu;
	connect_menu = new wxMenu;
	help_menu = new wxMenu;

    file_menu->Append(CONTROLS_OPTIONS, _T("&Options"), _T("Options"));
    file_menu->Append(CONTROLS_ADMIN, _T("&Administration"), _T("Administration"));
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
	toolbar = CreateToolBar( wxTB_HORIZONTAL|wxNO_BORDER  | wxTB_FLAT);
	
	
	int x = 0, y = 0;
	GetClientSize(&x, &y);
	bitmap_title = new wxStaticBitmap(toolbar, -1,
									wxBitmap("./icons/titre.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(x - 520 ,0),
									wxDefaultSize
									);
	degrade_title = new wxStaticBitmap(toolbar, -1,
									wxBitmap("./icons/degrade_titre.bmp", wxBITMAP_TYPE_BMP),
									wxPoint(x - 290 , 0),
									wxDefaultSize
									);
	wxBitmap toolBarBitmaps[7];
	toolBarBitmaps[0] = wxBitmap("./icons/connect.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[1] = wxBitmap("./icons/about.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[2] = wxBitmap("./icons/options.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[3] = wxBitmap("./icons/send_broadcast.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[4] = wxBitmap("./icons/client_connect.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[5] = wxBitmap("./icons/disconnect.bmp",wxBITMAP_TYPE_BMP);
	toolBarBitmaps[6] = wxBitmap("./icons/admin.bmp",wxBITMAP_TYPE_BMP);

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
	wxMask *mask6 = new wxMask(toolBarBitmaps[6], *wxWHITE);
    toolBarBitmaps[6].SetMask(mask6);

	toolbar->SetToolBitmapSize(wxSize(toolBarBitmaps[3].GetWidth(),toolBarBitmaps[3].GetHeight()));
	toolbar->AddTool (CONTROLS_CONNECT_CLIENT, "Connect to RedCap Client Consol", toolBarBitmaps[4], _T("Connect to RedCap Client Consol"));
	toolbar->AddTool (CONTROLS_CONNECT, "Connect to a server", toolBarBitmaps[0], _T("Connect to a server"));
	toolbar->AddTool (CONTROLS_DISCONNECT, "Disconnection", toolBarBitmaps[5], _T("Disconnect the Server"));
	toolbar->AddTool (CONTROLS_OPTIONS, "Configuration", toolBarBitmaps[2], _T("Configuration"));
	toolbar->AddTool (CONTROLS_ADMIN, "Administration", toolBarBitmaps[6], _T("Administration"));
	toolbar->AddTool (CONTROLS_BROADCAST, "Send a Broadcast", toolBarBitmaps[3], _T("Send Broadcast"));
	toolbar->AddTool(CONTROLS_ABOUT, "About RedCap", toolBarBitmaps[1], _T("About RedCap"));
	toolbar->Realize();
}

void MyFrame::OnClearLog(wxCommandEvent& WXUNUSED(event))
{
}


void MyFrame::OnMove( wxMoveEvent& event )
{
    event.Skip();
}

void MyFrame::OnSize( wxSizeEvent& event )
{
  event.Skip();
}

void MyFrame::OnIdle( wxIdleEvent& WXUNUSED(event) )
{
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

void MyFrame::OnConnect_client(wxCommandEvent &event)
{
	Connexion_Client dialog_client(this, -1, wxT("RedCAP client connection"), wxDefaultPosition);
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
		m_sock->Connect(addr1, FALSE);
		m_sock->WaitOnConnect(1);
		
		if (m_sock->IsConnected())
		{
			is_connected_console = 1;
			connect_menu->Enable(CONTROLS_CONNECT, TRUE);
			connect_menu->Enable(CONTROLS_CONNECT_CLIENT, FALSE);
			SetStatusText("Connection : connected to RedCap client console.", 1);

			file_menu->Enable(CONTROLS_OPTIONS, TRUE);
			file_menu->Enable(CONTROLS_ADMIN, FALSE);
			connect_menu->Enable(CONTROLS_CONNECT, TRUE);
			connect_menu->Enable(CONTROLS_DISCONNECT, FALSE);
			connect_menu->Enable(CONTROLS_CONNECT_CLIENT, FALSE);
			m_panel->Enable(false);
			toolbar->EnableTool(CONTROLS_CONNECT_CLIENT, false);
			toolbar->EnableTool(CONTROLS_CONNECT, true);
			toolbar->EnableTool(CONTROLS_DISCONNECT, false);
			toolbar->EnableTool(CONTROLS_OPTIONS, true);
			toolbar->EnableTool(CONTROLS_BROADCAST, false);
			m_panel->panel_public->SendChatButton->Enable(false);
			m_panel->panel_transfer->ExploClient->download_button->Enable(false);
			m_panel->panel_transfer->ExploClient->upload_button->Enable(false);
			m_panel->panel_transfer->ExploServer->delete_button->Enable(false);
			m_panel->panel_transfer->ExploServer->create_folder_button->Enable(false);
			m_panel->panel_transfer->ExploServer->preview_button->Enable(false);

			wxMessageBox("You are connected to RedCap Console");
			//demande la liste des servers
			output_message("list_server");
		}
		else
		{
			m_sock->Close();
			wxMessageBox(_("Can't connect to the RedCap Consol Client\nPlease check the HostName and the port."), _("Alert !"));
		}
	}
}

void MyFrame::OnOptions(wxCommandEvent &event)
{
    COptions  dialog(this, -1, wxT("RedCAP options"), wxDefaultPosition);
  	dialog.CentreOnParent();
  	dialog.ShowModal();
}

void MyFrame::OnAdmin(wxCommandEvent &event)
{
	output_message("get_login_list");
	m_admin = new CAdmin(this, -1, wxT("RedCAP Administration"), wxDefaultPosition);
	m_admin->CentreOnParent();
	m_admin->Show(TRUE);
}

void MyFrame::OnAbout( wxCommandEvent& WXUNUSED(event) )
{
	wxMessageDialog dialog(this, _T("Made By :\n- Nicolas Baret\n- Christopher Premaillon\n- Gregory Koeff\n- Hansi Pierre\n- Michel Le Cocq\n\nwww.caps-project.com\n\nEpitech 2004"), _T("CAPS Project V1.0"), wxOK | wxICON_INFORMATION );
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
	m_panel->List_users->DeleteAllItems();
	m_panel->panel_transfer->ExploServer->tree_list->DeleteAllItems();
	m_panel->panel_transfer->ExploClient->tree_list->DeleteAllItems();
	outputcmd.push_back("pwd");
	outputcmd.push_back("ls");
	outputcmd.push_back("lls");
	outputcmd.push_back("get_user_list");
}


void MyFrame::input_message(wxSocketEvent& event)
{
	vector<wxString> tab;
	wxString stmpbuf;


	m_busy = TRUE;
	while (m_sock->IsData())
	{
		busy = true;
		m_sock->Read(tmpbuf, 3000);
		int lastcount = m_sock->LastCount();
		tmpbuf[lastcount] = '\0';
	//busy = false;
	//////////////// test pour ecrire ce qu'on lit sur le chat ///////////////////
	//m_panel->panel_public->TopChatWindow->SetDefaultStyle(wxTextAttr(*wxBLACK));
	//char *zzz = new char[35];
	//sprintf(zzz, "((lu_%d_", lastcount);
	//m_panel->panel_public->TopChatWindow->AppendText(zzz);
	//sprintf(zzz, "curr size_%d_", preview_curr_size);
	//m_panel->panel_public->TopChatWindow->AppendText(zzz);
	//sprintf(zzz, "total size_%d_))\n", preview_size);
	//m_panel->panel_public->TopChatWindow->AppendText(zzz);
	//////////////////////////////////////////////////////////////////////////////

		if (opt_crypto) // if crypto enabled
			stmpbuf = crypto(tmpbuf, Pass_crypto, 'd');
		else
			stmpbuf = wxString(tmpbuf);
		//wxMessageBox(" texte lu sur la socket\n" + stmpbuf);
		buf = buf + stmpbuf;
		stmpbuf.clear();
        if (preview_size > 1)
		{
			//wxMessageBox("remplissage...");

			preview_in_file(tmpbuf, lastcount);
			buf.clear();
			tab_line_cmd.clear();
			busy = false;
			return;
		}
		tab_line_cmd = StringToTab(wxString(buf), "\n");
		if (tab_line_cmd.size() > 0 && !preview_size)
		{
			for ( vector<wxString>::iterator i = tab_line_cmd.begin(); i != tab_line_cmd.end(); ++i)
			{
				line_cmd = *i;
				if (!wxStrncmp(line_cmd, "preview:", 8))
				{
					preview_size = 1;
				}

				if (!wxStrncmp(line_cmd, "An other IHM", 12))
				  {
				    buf.clear();
				    tab_line_cmd.clear();
				    lost_console();
				    busy = false;
				    return;
				  }	




			}
		}

		if (tab_line_cmd.size() > 0 && preview_size == 1)
		{
			for ( vector<wxString>::iterator i = tab_line_cmd.begin(); i != tab_line_cmd.end(); ++i)
			{
				line_cmd = *i;
				if (!wxStrncmp(line_cmd, "receive:", 8))
				{
					int n;

					line_cmd = line_cmd.AfterFirst(':');
					preview_size = atol(line_cmd);
					preview_curr_size = 0;

					for (n = 0; tmpbuf[n] && strncmp(tmpbuf + n, "receive:", 8); n++)
						;
					if (!tmpbuf[n]) break;
					for (++n; tmpbuf[n] && tmpbuf[n] != '\n'; n++)
						;
					if (!tmpbuf[n]) break;
					tmpbuf = tmpbuf + n + 1;

					preview_in_file(tmpbuf, lastcount - n - 1);
					buf.clear();
					tab_line_cmd.clear();
					busy = false;
					return;
				}
			}
		}

		if (!wxStrcmp(buf.substr(buf.length() - 4, 3), "] %") && !preview_size)
		{
			//////////////// test pour ecrire ce qu'on lit sur le chat ///////////////////
			m_panel->panel_public->TopChatWindow->SetDefaultStyle(wxTextAttr(*wxBLACK));
			m_panel->panel_public->TopChatWindow->AppendText(buf);
			m_panel->panel_public->TopChatWindow->AppendText("(---)");
			//wxMessageBox(buf);
			//////////////////////////////////////////////////////////////////////////////

			tab_line_cmd = StringToTab(wxString(buf), "\n");
			for ( vector<wxString>::iterator i = tab_line_cmd.begin(); i != tab_line_cmd.end(); ++i )
			{
				line_cmd = *i;
				tab_arg_line_cmd.clear();
				tab_arg_line_cmd = StringToTab(line_cmd, " ");

			if (tab_arg_line_cmd.size())
			{
				if (!wxStrncmp(line_cmd, "[", 1) && !wxStrcmp(line_cmd.substr(line_cmd.length() - 4, 3), "] %"))
				{
					prompt.clear();
					//line_cmd = "[test] % [test] % ";
					if (line_cmd.length() > 10 && !wxStrncmp(line_cmd.substr((line_cmd.length() / 2) - 4), "] % ", 4))
						prompt = wxStrdup(line_cmd.substr(0, (line_cmd.length() / 2)));
					else
						prompt = wxStrdup(line_cmd);
				}

				wxString tmp = tab_arg_line_cmd.at(0);
				//wxMessageBox("premire mot   " + tmp);
				if (tab_arg_line_cmd.size() > 2 &&  !wxStrcmp(tmp, "notify") && !wxStrcmp(tab_arg_line_cmd.at(1), "change") && !wxStrcmp(tab_arg_line_cmd.at(2), "user:"))
					user_change(i);
				if (tab_arg_line_cmd.size() > 2 &&  !wxStrcmp(tmp, "notify") && !wxStrcmp(tab_arg_line_cmd.at(1), "delete") && !wxStrcmp(tab_arg_line_cmd.at(2), "user") && !wxStrcmp(tab_arg_line_cmd.at(3), "id"))
					user_delete(tab_arg_line_cmd.at(4));
				if (!wxStrcmp(tmp, "relogin"))
					relogin();
				if (!wxStrcmp(tmp, "disconnecting"))
					disconnecting(i);
				if (tab_arg_line_cmd.size() == 2 && !wxStrcmp(tab_arg_line_cmd.at(0), "server") && !wxStrncmp(tab_arg_line_cmd.at(1),"version:", 8))
					server_version(i);
				if (tab_arg_line_cmd.size() > 6 && !wxStrcmp(tab_arg_line_cmd.at(0), "server") && !wxStrcmp(tab_arg_line_cmd.at(2),"is") && !wxStrcmp(tab_arg_line_cmd.at(3),"now") && !wxStrcmp(tab_arg_line_cmd.at(4),"know") && !wxStrcmp(tab_arg_line_cmd.at(5),"as"))
					server_known(i);
				if (!wxStrcmp(tmp, "disconnected"))
					disconnected(i);
				if (!wxStrcmp(tmp, "server_list:"))
					server_list(i);
				if (!wxStrcmp(tmp, "my_access:"))
					user_access(i);
				if (!wxStrcmp(tmp, "connected"))
					connect_server(i);
				if (!wxStrcmp(tmp, "Agreement:") && !connect_opt_ag)
					agree();
				if (!wxStrcmp(tmp, "Agreement:") && connect_opt_ag) 
					auto_agreement();
				if (!wxStrcmp(tmp, "user_list:"))
					user_set_list(i);
				if (!wxStrcmp(tmp, "set_files_list:"))
					file_set_list(i);
				if (!wxStrcmp(tmp, "login_list:"))
					get_login_list(i);
				if (!wxStrcmp(tmp, "set_client_files_list:"))			
					file_client_set_list(i);
				if (!wxStrcmp(tmp, "receive_chat:"))
					receive_chat(i);
 				if (!wxStrcmp(tmp, "receive_priv_chat"))
					receive_priv_chat(i);
				if (!wxStrcmp(tmp, "broadcast_msg"))
					broadcast_msg(i);
				if (!wxStrcmp(tmp, "receive_server_msg:"))
					receive_server_msg(i);
				if (!wxStrcmp(tmp, "task:"))
					task_list(i);
				if (!wxStrcmp(tmp, "pwd:"))
					m_panel->panel_transfer->ExploServer->PathBar->SetValue(*(i+1));
				if (!wxStrcmp(tmp, "file_info:"))
					AffFileInfo(i);
				if (!wxStrcmp(tmp, "error:"))
					error_msg(i);
				if (!wxStrcmp(tmp, "get_user:"))
					get_user(i);
			}

		}
		buf.clear();
		tab_line_cmd.clear();
		busy = false;
		}
	}
	if (outputcmd.size())
	{
		wxString tempcmd;

		tempcmd = *outputcmd.begin();
		outputcmd.erase(outputcmd.begin());
		//wxMessageBox("un message en atente va etre expedi�! \n" + tempcmd);
		output_message(tempcmd);
	}
	m_busy = FALSE;
}
void MyFrame::output_message(wxString str)
{
	int len;
//	char *temp;
	wxString msg;
	wxString aff;

//wxMessageBox("outPUTE");

	//if (is_connected_console == 0)
	//	return;

	if (busy == false)
	{
	//if (opt_crypto) // if crypto enabled
	//{
	//	temp = crypto( str, Pass_crypto, 'c');
	//	msg = wxStrdup(temp);
	//	msg = msg + "12";
	//	len = wxStrlen(msg);
	//	msg.at(len - 2) = 13;
	//	msg.at(len - 1) = 10;

	//}
	//else
	//{
		msg = str + "12";
		len = wxStrlen(msg);
		msg.at(len - 2) = 13;
		msg.at(len - 1) = 10;
	//}
	

	aff = "j'envoie au console : [";
	aff += msg;
	aff += "]";
	//wxMessageBox(aff);

	m_busy = TRUE;
	m_sock->Write(msg, len);
	m_busy = FALSE;
	//delete temp;
	}
	else
	{
		//wxMessageBox("attend pour ecrire!! la commande\n" + str);
		outputcmd.push_back(str);
	}
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
		for  (c = b; c < chaine.length() && !wxStrncmp(chaine.substr(c, chaine.length() - c) ,  sep, sep.length())         ; c++)
			;
		a = c;
	}
	return tab;
}

/*MyFrame::user_account()
{
	//  parser la chaine 
	//	user_account:
	//	toto
	//	titi est bo
	//  tutu
	//  [192.168.5.16 2]

	CAdmin  dialog(this, -1, wxT("RedCAP Administration"), wxDefaultPosition);
  	dialog.CentreOnParent();
  	dialog.ShowModal();
}*/
