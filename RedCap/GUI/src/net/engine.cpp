#include "general.h"

void MyFrame::connect_server(vector<wxString>::iterator i)
{
	//wxMessageBox("connect_ok");
	if (tab_arg_line_cmd.size() < 3)
		return;
	connexion_name = wxStrdup(tab_arg_line_cmd.at(2));
	if (wxStrcmp(tab_arg_line_cmd.at(3), "with"))
		connexion_capsproto = true;
	else
		connexion_capsproto = false;	
}


void MyFrame::relogin()
{
	//wxMessageBox("relogin");
}
void MyFrame::disconnecting(vector<wxString>::iterator i)
{
	//wxMessageBox("disconnecting");
}
void MyFrame::server_version(vector<wxString>::iterator i)
{
	wxString			line;
	long				n;
	int					x;
	CServData			*tmp;
	CPrivChat			*privchat;

	//wxMessageBox("serverversion");

	if (!wxStrcmp(connexion_name, ""))
		return;

	///////////////////////////////////////////////////////////////////////////////////////
	// sauve la config dans un element de la liste, si il existe pas, on le cree

	//si on est deja connecte a un serveur, on le recherche ses donnees pour les actualiser
	tmp = m_panel->Current_serv_data;
	if (tmp)
	{
		//phase de sauvegarde
		tmp->CurentSelection = m_panel->m_notebook->GetSelection();
		//sauv public chat
		tmp->PublicChat = m_panel->panel_public->TopChatWindow->GetValue();
		//sauv titre et contenu des chat priv<E9>s
		tmp->PrivChat.clear();
		tmp->PrivChatTitle.clear();
		tmp->PrivChatId.clear();
		for ( vector<CPrivChat *>::iterator it = m_panel->List_PrivChat.begin(); it != m_panel->List_PrivChat.end(); ++it )
		{
			privchat = *it;
			tmp->PrivChatTitle.push_back(privchat->name);
			tmp->PrivChat.push_back(privchat->TopChatWindow->GetValue());
			tmp->PrivChatId.push_back(privchat->id_user);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// on va parcourir les servdata existants.
	// si on n'en trouve pas un qui a le meme prompt on ajoute celui la
	CServData *listdata;
	n = 0;
	for(vector<CServData *>::iterator data = m_panel->Serv_Data.begin(); data != m_panel->Serv_Data.end(); data++)
	{
		listdata = *data;
		if (!wxStrcmp(listdata->Serv_prompt, prompt))
		{
			n = 1;
			break;
		}
	}
	if (n == 0)
	{

		///////////////////////////////////////////////////////////////////////////////////////
		//phase de reinitialisation de l'affichage (on reinitialise tout car on se connecte a un nouveau serveur)
		m_panel->List_users->ClearAll();
		m_panel->panel_public->TopChatWindow->Clear();
		m_panel->List_PrivChat.clear(); 
		m_panel->panel_transfer->ExploServer->tree_list->DeleteAllItems();
		m_panel->panel_transfer->ExploClient->tree_list->DeleteAllItems();
		x = m_panel->m_notebook->GetPageCount();
		for (n =  m_panel->nb_tab; n < x;n++) 
			m_panel->m_notebook->DeletePage(m_panel->m_notebook->GetPageCount() - 1);

		//////////////////////////////////////////////////////////////////////////////
		//creation d'un nouveau serv Data pour le nouveau serveur
		tmp = new CServData;
		tmp->Serv_prompt = prompt;
		tmp->Serv_ip = connexion_name;
		tmp->Serv_name = connexion_name;
		tmp->Serv_capsproto = connexion_capsproto;
		line = *i;
		if (line.size() > 15)
			tmp->Server_version = line.substr(15,line.size() - 15);
		tmp->CurentSelection = 0;
		tmp->Users.clear();
		tmp->PublicChat = wxEmptyString;
		tmp->PrivChatTitle.clear();
		tmp->PrivChat.clear();
		tmp->PrivChatId.clear();
		tmp->Server_version = "";
		tmp->user_type = 0;

		m_panel->Serv_Data.push_back(tmp);

		//on precise a m_panel le serveur ou on se trouve
		m_panel->Current_serv_data = m_panel->Serv_Data.at(m_panel->Serv_Data.size() - 1);
		SetStatusText("Connection : connected to a server.", 1);
		// ajoute le nom du serveur dans la liste de gauche de la fenettre
		m_panel->List_serv->Append(tmp->Serv_prompt);
		m_panel->List_serv->SetSelection(m_panel->List_serv->GetCount() - 1);

		file_menu->Enable(CONTROLS_OPTIONS, TRUE);
		file_menu->Enable(CONTROLS_ADMIN, TRUE);
		connect_menu->Enable(CONTROLS_CONNECT, TRUE);
		connect_menu->Enable(CONTROLS_DISCONNECT, TRUE);
		connect_menu->Enable(CONTROLS_CONNECT_CLIENT, FALSE);
		m_panel->Enable(true);
		toolbar->EnableTool(CONTROLS_CONNECT_CLIENT, false);
		toolbar->EnableTool(CONTROLS_CONNECT, true);
		toolbar->EnableTool(CONTROLS_DISCONNECT, true);
		toolbar->EnableTool(CONTROLS_OPTIONS, true);
		toolbar->EnableTool(CONTROLS_BROADCAST, false);
		m_panel->panel_public->SendChatButton->Enable(false);
		m_panel->panel_transfer->ExploClient->download_button->Enable(false);
		m_panel->panel_transfer->ExploClient->upload_button->Enable(false);
		m_panel->panel_transfer->ExploServer->delete_button->Enable(false);
		m_panel->panel_transfer->ExploServer->create_folder_button->Enable(false);
		m_panel->panel_transfer->ExploServer->preview_button->Enable(false);
		//Affstruct();

		// on met login_in_progress a true commeca, la prochaine fois qu'on recevra un "my_acces" on saura qu'il faut faire output de ls et lls
		login_in_progress = true;
	}
	connexion_name = "";
}

void MyFrame::server_known(vector<wxString>::iterator i)
{
	wxString	line;
	wxString	name;

	//wxMessageBox("server_know");
#ifdef __WIN32__
	size_t		n;
	CServData	*tmp;
	line = *i;
	tmp = m_panel->Current_serv_data;
	name = "";
	for (n = 6; n < tab_arg_line_cmd.size(); n++)
	{
		name = name +  tab_arg_line_cmd.at(n);
		if (n < tab_arg_line_cmd.size() - 1)
			name = name + " ";
	}
	name = name.SubString(1, name.Len() - 2);
	tmp->Serv_prompt.Replace(tab_arg_line_cmd.at(1), name, TRUE);
	tmp->Serv_name = name;
	n = m_panel->List_serv->GetSelection();
	m_panel->List_serv->SetString(n, m_panel->Current_serv_data->Serv_prompt);
	m_panel->List_serv->SetSelection(n);
	//Affstruct();
#endif

//	wxString	line;
//	wxString	name;
//	size_t		n;
//	CServData	*tmp;
//
//	//wxMessageBox("server_know");
//	line = *i;
//	tmp = m_panel->Current_serv_data;
//	name = "";
//	for (n = 6; n < tab_arg_line_cmd.size(); n++)
//	{
//		name = name +  tab_arg_line_cmd.at(n);
//		if (n < tab_arg_line_cmd.size() - 1)
//			name = name + " ";
//	}
//	name = name.SubString(1, name.Len() - 2);
//	m_panel->Current_serv_data->Serv_prompt.Replace(tab_arg_line_cmd.at(1), name, TRUE);
//	tmp->Serv_name = name;
//	n = m_panel->List_serv->GetSelection();
//	//m_panel->List_serv->Replace(n - 1,n + 1, m_panel->Current_serv_data->Serv_prompt);
//#ifdef __WIN32__
//	m_panel->List_serv->SetString(n, m_panel->Current_serv_data->Serv_prompt);
//	m_panel->List_serv->SetSelection(n);
//#endif
//	//Affstruct();
}

void MyFrame::disconnected(vector<wxString>::iterator i)
{
	int			x;
	int			n;
	wxString	str;

	//wxMessageBox("fonction disconnected");

	//*******************************************************************************
	//phase de reinitialisation de l'affichage (on reinitialise tout car on se connecte a un nouveau serveur)
	m_panel->List_users->ClearAll();
	m_panel->panel_public->TopChatWindow->Clear();
	m_panel->List_PrivChat.clear(); 
	m_panel->panel_transfer->ExploServer->tree_list->DeleteAllItems();
	m_panel->panel_transfer->ExploClient->tree_list->DeleteAllItems();
	x = m_panel->m_notebook->GetPageCount();
	for (n =  m_panel->nb_tab; n < x;n++) 
		m_panel->m_notebook->DeletePage(m_panel->m_notebook->GetPageCount() - 1);
////////////////////////////////////////////////////////////////////////////////////
	//sleep(1);
	output_message("list_server");
}
void MyFrame::user_delete(wxString id)
{
	output_message("get_user_list");
}

void MyFrame::error_msg(vector<wxString>::iterator i)
{
	wxString line;

	line = *i;
	wxMessageBox(line);
}

void MyFrame::Affstruct()
{
	wxString				msg;
	wxString				str;
	wxString				str2;
	int						user_id;
	char				msg2[9];
	CServData			*dataserv;
	CUser				*nuser;

	msg = "contenu de la structure\n";
	dataserv = NULL;
	for ( vector<CServData * >::iterator it = m_panel->Serv_Data.begin(); it != m_panel->Serv_Data.end(); ++it )
	{
		dataserv = *it;
		msg = msg + "serveur/////////////////////////////\n";
		msg = msg + "    nom du serveur -" + dataserv->Serv_name + "-\n";
		msg = msg + "    version -" + dataserv->Server_version + "-\n";
		msg = msg + "    pompt -" + dataserv->Serv_prompt + "-\n";
		msg = msg + "    ip -" + dataserv->Serv_ip + "-\n";
		nuser = NULL;
		msg = msg + "---- users ------\n";
		for (vector<CUser *> ::iterator its = dataserv->Users.begin(); its != dataserv->Users.end();  its++)
		{
			nuser = *its;
			msg = msg + "     " + nuser->name;
			sprintf(msg2, "%d", nuser->id);
			msg = msg + "  " + msg2;
			msg = msg + "  " + nuser->icon + "\n";
		}
		vector<wxString>::iterator VPrivChat = dataserv->PrivChat.begin();
		vector<wxString>::iterator VPrivChatTitle = dataserv->PrivChatTitle.begin();
		vector<int>::iterator VPrivChatId = dataserv->PrivChatId.begin();
		for (; VPrivChat != dataserv->PrivChat.end()
			&& VPrivChatTitle != dataserv->PrivChatTitle.end()
			&& VPrivChatId != dataserv->PrivChatId.end();
			++VPrivChat, 
			++VPrivChatTitle, 
			++VPrivChatId)
		{
			str = *VPrivChat;
			str2 = *VPrivChatTitle;
			user_id = *VPrivChatId;
			sprintf(msg2, "%d", user_id);
			msg = msg + "chat priv� avec " + str2 + " id " + msg2 + "\n";		
			msg = msg + "    " + str +  "\n";
		}
	}
	wxMessageBox(msg);
}

void MyFrame::change_serv(wxCommandEvent& event)
{
	CServData	*ancienData;
	CPrivChat	*privchat;

	if (m_panel->List_serv->GetCount() < 2 || !wxStrcmp(m_panel->Current_serv_data->Serv_prompt, m_panel->List_serv->GetValue()))
        return;
	ancienData = m_panel->Current_serv_data;
	//*******************************************************************************
	//phase de sauvegarde
	ancienData->CurentSelection = m_panel->m_notebook->GetSelection();
	//sauv public chat
	ancienData->PublicChat = m_panel->panel_public->TopChatWindow->GetValue();
	//sauv titre et contenu des chat priv�s
	ancienData->PrivChat.clear();
	ancienData->PrivChatTitle.clear();
	ancienData->PrivChatId.clear();
	for ( vector<CPrivChat *>::iterator it = m_panel->List_PrivChat.begin(); it != m_panel->List_PrivChat.end(); ++it )
	{
		privchat = *it;
		ancienData->PrivChatTitle.push_back(privchat->name);
		ancienData->PrivChat.push_back(privchat->TopChatWindow->GetValue());
		ancienData->PrivChatId.push_back(privchat->id_user);
	}

	/////////////////////////////////////////////////////////////////////////////////
	// sauvegarde terminee, on actualise le CurrentServData
	CServData	*tmp;
	for ( vector<CServData * >::iterator it = m_panel->Serv_Data.begin(); it != m_panel->Serv_Data.end(); ++it )
	{
		tmp = *it;
		if(!wxStrcmp(tmp->Serv_prompt, m_panel->List_serv->GetValue()))
		{
			m_panel->Current_serv_data = tmp;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////
	// maintenant que le CurrentServData pointe sur leserveur d�sir�, on fait
	// list_server pour que la fonction fasse le reste du boulot
	output_message("list_server");
}

void MyFrame::server_list(vector<wxString>::iterator i)
{
	vector<wxString>	tab;
	wxString			line;
	wxString			my_prompt;
	wxString			my_console_prompt = "";

	vector<wxString>::iterator save_i;
	int					n;


	i++;
	save_i = i;
	if (m_panel->Current_serv_data)
		my_prompt = m_panel->Current_serv_data->Serv_prompt;
	else
		my_prompt = "";

	/////////////////////////////////////////////////////////////////////////////
	// on parcours tous les serveurs pour enrichir serv data si il le faut
	vector<wxString>::iterator PointerLine = i;
	for (; PointerLine != tab_line_cmd.end(); PointerLine++)
	{
		line = *PointerLine;
		if (!wxStrcmp(line, "")) break;
		tab = StringToTab(line, "\t");
		if (tab.size() < 4) break;

		wxString prompt = "[";
		wxString name = "";
		wxString ip = "";
		bool	 capsproto = false;
		wxString str;

		//////////////////////////////////////////////////////////////////////
		// parsing de la commande pour recuprer les serveur connect�s
		if (tab.size() == 6 && !wxStrcmp(tab.at(5), "Logged"))
		{
			name = tab.at(1).SubString(1, tab.at(1).Len() - 3);
			ip = tab.at(2).AfterFirst('@');
			ip = ip.BeforeFirst(':');
			if (wxStrcmp(name, "no nam"))
				prompt = prompt + name;
			else
				prompt = prompt + ip;
			prompt = prompt + " " + tab.at(0).BeforeFirst(':') + "] % ";
			if (!wxStrcmp(tab.at(4), "Caps:1"))
				capsproto = true;
			else
				capsproto = false;
			
			///////////////////////////////////////////////////////////////////////////
			// on va parcourir les servdata existants.
			// si on n'en trouve pas un qui a le meme prompt on ajoute celui la
			CServData *listdata;
			n = 0;
			for(vector<CServData *>::iterator data = m_panel->Serv_Data.begin(); data != m_panel->Serv_Data.end(); data++)
			{
				listdata = *data;
				if (!wxStrcmp(listdata->Serv_prompt, prompt))
				{
					n = 1;
					break;
				}
			}
			if (n == 0)
			{
				//////////////////////////////////////////////////////////////////////
				// creation d'un nouveau servdata
				CServData *nouvserv;
				nouvserv = new CServData;
				nouvserv->Serv_prompt = prompt;
				nouvserv->Serv_ip = ip;
				nouvserv->Serv_name = name;
				nouvserv->Serv_capsproto = capsproto;
				nouvserv->Server_version = "";
				nouvserv->CurentSelection = 0;
				nouvserv->Users.clear();
				nouvserv->PublicChat = wxEmptyString;
				nouvserv->PrivChat.clear();
				nouvserv->PrivChatTitle.clear();
				nouvserv->PrivChatId.clear();

				m_panel->Serv_Data.push_back(nouvserv);
				//////////////////////////////////////////////////////////////////////
				// creation d'un element dans la liste des serveurs
				m_panel->List_serv->Append(nouvserv->Serv_prompt);
			}
		}
	}
	my_console_prompt = wxStrdup(*PointerLine);
	//Affstruct();

	/////////////////////////////////////////////////////////////////////////////
	// on parcours tous les serveurs pour virer de listservdata ceux qui sont pas loggues loggu�
	i = save_i;
	for (vector<wxString>::iterator PointerLine = i; PointerLine != tab_line_cmd.end(); PointerLine++)
	{
		line = *PointerLine;
		if (!wxStrcmp(line, "")) break;
		tab = StringToTab(line, "\t");
		if (tab.size() < 4) break;

		wxString prompt = "[";
		wxString name = "";
		wxString ip = "";

		//////////////////////////////////////////////////////////////////////
		// parsing de la ligne pour recuprer les infos 
		if (tab.size() == 6)
		{
			name = tab.at(1).SubString(1, tab.at(1).Len() - 3);
			ip = tab.at(2).AfterFirst('@');
			ip = ip.BeforeFirst(':');
			if (wxStrcmp(name, "no nam"))
				prompt = prompt + name;
			else
				prompt = prompt + ip;
			prompt = prompt + " " + tab.at(0).BeforeFirst(':') + "] % ";


			/////////////////////////////////////////////////////////////////////////////
			// si le serveur est deconnect�, on le supprime de listservdata 
			if (wxStrcmp("Logged", tab.at(5)))
			{
				//wxMessageBox("je vire " + tab.at(0) + " prompt " + prompt + " car " + tab.at(5));
				CServData *tmpdata;
				n = 0;
				for (vector<CServData * >::iterator it = m_panel->Serv_Data.begin(); it != m_panel->Serv_Data.end(); ++it )
				{
					tmpdata = *it;
					if(!wxStrcmp(tmpdata->Serv_prompt, prompt))
					{
						m_panel->Serv_Data.erase(m_panel->Serv_Data.begin() + n);
						m_panel->List_serv->Delete(n);
						m_panel->List_serv->SetSelection(0);
						break;
					}
					n++;
				}
			}
		}
	}

//	Affstruct();
	/////////////////////////////////////////////////////////////////////////////
	// on reparcours listservdata pour voir si on est toujours dedans ou pas
	CServData *tmpdata;
	n = 0;
	for (vector<CServData * >::iterator it = m_panel->Serv_Data.begin(); it != m_panel->Serv_Data.end(); ++it )
	{
		tmpdata = *it;
		if(!wxStrcmp(tmpdata->Serv_prompt, my_prompt))
		{
			n = 1;
			break;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////
	// si on est pas dedans ca veut dire qu'on s'est fait d�connect�
	if(n == 0)
	{
		m_panel->Current_serv_data = NULL;
		/////////////////////////////////////////////////////////////////////////////
		// comme on est deconnect�, on va essayer de pointer vers le premier serveur logg�

		// si servdata est nul, ca veut dire qu'il y a pas d'autres serveurs logg�s
		if (m_panel->Serv_Data.size() < 1)
		{
			SetStatusText("Connection : connected to RedCap client console.", 1);
			file_menu->Enable(CONTROLS_OPTIONS, TRUE);
			file_menu->Enable(CONTROLS_ADMIN, TRUE);
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
		}
		else
		{
			///////////////////////////////////////////////////////////////////////////
			// comme servdata est pas nul, on va faire pointer le client graphique sur le premier serveur venu
			m_panel->List_serv->SetSelection(0);
			m_panel->Current_serv_data = m_panel->Serv_Data.at(0);
			wxString my_nouv_prompt = m_panel->Current_serv_data->Serv_prompt;
			///////////////////////////////////////////////////////////////////////////
			// maintenant, on va faire pointer le client console sur le meme serveur que le client graphique
			n = 0;
			int my_prompt_nbr = 0;
			int my_console_prompt_nbr = 0;
			i = save_i;
			for (vector<wxString>::iterator PointerLine = i; PointerLine != tab_line_cmd.end(); PointerLine++)
			{
				line = *PointerLine;
				if (!wxStrcmp(line, "")) break;
				tab = StringToTab(line, "\t");
				if (tab.size() < 4) break;

				wxString prompt = "[";
				wxString name = "";
				wxString ip = "";

				//////////////////////////////////////////////////////////////////////
				// parsing de la ligne pour recuprer les infos 
				if (tab.size() == 6)
				{
					name = tab.at(1).SubString(1, tab.at(1).Len() - 3);
					ip = tab.at(2).AfterFirst('@');
					ip = ip.BeforeFirst(':');
					if (wxStrcmp(name, "no nam"))
						prompt = prompt + name;
					else
						prompt = prompt + ip;
					prompt = prompt + " " + tab.at(0).BeforeFirst(':') + "] % ";
					if (!wxStrcmp(prompt, my_nouv_prompt))
						my_prompt_nbr = n;
					if (!wxStrcmp(prompt, my_console_prompt))
						my_console_prompt_nbr = n;
				}
				n++;
			}
			n = my_console_prompt_nbr - my_prompt_nbr;
			if (n > 0)
			{
				int m;
				for (m = 0; m < n; m ++)
					output_message("prev_server");
			}
			if (n < 0)
			{
				int m;
				for (m = 0; m > n; m --)
					output_message("next_server");
			}

			///////////////////////////////////////////////////////////////////////////
			// maintenant on s'occupe de l'affichage
			// si my_prompt est "" ca veut dire que le client graphique vient de se lancer
			// dans ce cas tout le contenu de listServData est nouveau et on doit redemander les droits...
			if (!wxStrcmp(my_prompt, ""))
			{
				//////////////////////////////////////////////////////////////////////
				// on corrige les droits par defaults
				file_menu->Enable(CONTROLS_OPTIONS, FALSE);
				file_menu->Enable(CONTROLS_ADMIN, TRUE);
				connect_menu->Enable(CONTROLS_CONNECT, TRUE);
				connect_menu->Enable(CONTROLS_DISCONNECT, TRUE);
				connect_menu->Enable(CONTROLS_CONNECT_CLIENT, TRUE);
				m_panel->Enable(true);
				toolbar->EnableTool(CONTROLS_CONNECT_CLIENT, false);
				toolbar->EnableTool(CONTROLS_CONNECT, true);
				toolbar->EnableTool(CONTROLS_DISCONNECT, true);
				toolbar->EnableTool(CONTROLS_OPTIONS, true);
				toolbar->EnableTool(CONTROLS_BROADCAST, false);
				m_panel->panel_public->SendChatButton->Enable(false);
				m_panel->panel_transfer->ExploClient->download_button->Enable(false);
				m_panel->panel_transfer->ExploClient->upload_button->Enable(false);
				m_panel->panel_transfer->ExploServer->delete_button->Enable(false);
				m_panel->panel_transfer->ExploServer->create_folder_button->Enable(false);
				m_panel->panel_transfer->ExploServer->preview_button->Enable(false);
				//////////////////////////////////////////////////////////////////////
				// on envoie quelques commandes
				output_message("my_access");
				output_message("pwd");
				output_message("ls");
				output_message("lls");
				output_message("get_user_list");
			}
			else
			{
				CServData *nouveauData;
				int x = 0;
				int y = 0;
				wxString	str;
				wxString	str2;
				int			user_id;
				CPrivChat	*privchat;

				nouveauData = m_panel->Current_serv_data;
				//phase d'actualisation
				m_panel->Current_serv_data = nouveauData;
				m_panel->List_users->ClearAll();
				m_panel->panel_public->TopChatWindow->Clear();
				//actualise les users
				output_message("get_user_list");
				//actualise le public chat
				m_panel->panel_public->TopChatWindow->WriteText(nouveauData->PublicChat);
				//actualise le private chat
				m_panel->List_PrivChat.clear(); //efface la liste
				x = m_panel->m_notebook->GetPageCount();
				for (n = m_panel->nb_tab; n < x;n++) //clear l'affichage
					m_panel->m_notebook->DeletePage(m_panel->m_notebook->GetPageCount() - 1);
				vector<wxString>::iterator VPrivChat = nouveauData->PrivChat.begin();
				vector<wxString>::iterator VPrivChatTitle = nouveauData->PrivChatTitle.begin();
				vector<int>::iterator VPrivChatId = nouveauData->PrivChatId.begin();
				for (; VPrivChat != nouveauData->PrivChat.end() 
					&& VPrivChatTitle != nouveauData->PrivChatTitle.end() 
					&& VPrivChatId != nouveauData->PrivChatId.end();
					++VPrivChat, ++VPrivChatTitle, ++VPrivChatId)
				{
					str = *VPrivChat;
					str2 = *VPrivChatTitle;
					user_id = *VPrivChatId;
					privchat = new CPrivChat(m_panel->m_notebook, str2 , x, y   ); //on cree une classe temporaire
					privchat->TopChatWindow->SetFont(wxFont(wxFontInfo(8).Family(wxFONTFAMILY_DECORATIVE).Style(wxFONTSTYLE_NORMAL).Weight(wxFONTWEIGHT_BOLD).FaceName("Unicode")));
					privchat->name = str2;
					privchat->TopChatWindow->SetValue(str);
					privchat->id_user = user_id;
					GetClientSize( &x, &y ); // on met tout a la bonne taille
					privchat->BitmapDegr->SetSize(160,0, x - 365, 24);
					privchat->splitter->SetSize(2, 26, x - 13 - WIDTH_UTILISATEUR, y - 64);
					privchat->splitter->SetSashPosition(y - 115);
					privchat->y_sash = privchat->splitter->GetSashPosition();
					privchat->SendChatButton->SetSize(x - 275, (y  - privchat->y_sash - 67) / 2 - 24, 86, 49 );
					privchat->CloseButton->SetSize(x - WIDTH_UTILISATEUR - 35, 0, 24, 24);
					privchat->ChatText->SetSize(0, 0, x - 113 - WIDTH_UTILISATEUR, y  - privchat->y_sash - 67);
					m_panel->List_PrivChat.push_back(privchat); //on enregistre la classe dans la liste
					//delete privchat;
				}
				m_panel->m_notebook->SetSelection(nouveauData->CurentSelection);
				//actualise les droits
				if (nouveauData->openuser)
					toolbar->EnableTool(CONTROLS_ADMIN, true);
				else
					toolbar->EnableTool(CONTROLS_ADMIN, false);

				if (nouveauData->broadcast)
					toolbar->EnableTool(CONTROLS_BROADCAST, true);
				else
					toolbar->EnableTool(CONTROLS_BROADCAST, false);
				if (nouveauData->sendchat)
					m_panel->panel_public->SendChatButton->Enable(true);
				else
					m_panel->panel_public->SendChatButton->Enable(false);
				if (nouveauData->downloadfile)
					m_panel->panel_transfer->ExploClient->download_button->Enable(true);
				else
					m_panel->panel_transfer->ExploClient->download_button->Enable(false);
				if (nouveauData->uploadfile)
					m_panel->panel_transfer->ExploClient->upload_button->Enable(true);
				else
					m_panel->panel_transfer->ExploClient->upload_button->Enable(false);
				if (nouveauData->deletefile)
					m_panel->panel_transfer->ExploServer->delete_button->Enable(true);
				else
					m_panel->panel_transfer->ExploServer->delete_button->Enable(false);
				if (nouveauData->createfolder)
					m_panel->panel_transfer->ExploServer->create_folder_button->Enable(true);
				else
					m_panel->panel_transfer->ExploServer->create_folder_button->Enable(false);
				if (nouveauData->downloadfile)
					m_panel->panel_transfer->ExploServer->preview_button->Enable(true);
				else
					m_panel->panel_transfer->ExploServer->preview_button->Enable(false);
			}


		}
	}
	else
	///////////////////////////////////////////////////////////////////////////
	// si on est dedans, il faut juste faire pointer le client console et vers le bon serveur
	// et changer l'affichage
	{
		wxString my_nouv_prompt = m_panel->Current_serv_data->Serv_prompt;
		///////////////////////////////////////////////////////////////////////////
		// maintenant, on va faire pointer le client console sur le meme serveur que le client graphique
		n = 0;
		int my_prompt_nbr = 0;
		int my_console_prompt_nbr = 0;
		i = save_i;
		for (vector<wxString>::iterator PointerLine = i; PointerLine != tab_line_cmd.end(); PointerLine++)
		{
			line = *PointerLine;
			if (!wxStrcmp(line, "")) break;
			tab = StringToTab(line, "\t");
			if (tab.size() < 4) break;

			wxString prompt = "[";
			wxString name = "";
			wxString ip = "";

			//////////////////////////////////////////////////////////////////////
			// parsing de la ligne pour recuprer les infos 
			if (tab.size() == 6)
			{
				name = tab.at(1).SubString(1, tab.at(1).Len() - 3);
				ip = tab.at(2).AfterFirst('@');
				ip = ip.BeforeFirst(':');
				if (wxStrcmp(name, "no nam"))
					prompt = prompt + name;
				else
					prompt = prompt + ip;
				prompt = prompt + " " + tab.at(0).BeforeFirst(':') + "] % ";
				if (!wxStrcmp(prompt, my_nouv_prompt))
					my_prompt_nbr = n;
				if (!wxStrcmp(prompt, my_console_prompt))
					my_console_prompt_nbr = n;
			}
			n++;
		}
		n = my_console_prompt_nbr - my_prompt_nbr;
		if (n > 0)
		{
			int m;
			for (m = 0; m < n; m ++)
				output_message("prev_server");
		}
		if (n < 0)
		{
			int m;
			for (m = 0; m > n; m --)
				output_message("next_server");
		}

		///////////////////////////////////////////////////////////////////////////
		// maintenant on s'occupe de l'affichage
		CServData *nouveauData;
		int x = 0;
		int y = 0;
		wxString	str;
		wxString	str2;
		int			user_id;
		CPrivChat	*privchat;

		nouveauData = m_panel->Current_serv_data;
		//phase d'actualisation
		m_panel->Current_serv_data = nouveauData;
		m_panel->List_users->ClearAll();
		m_panel->panel_public->TopChatWindow->Clear();
		//actualise les users
		output_message("get_user_list");
		//actualise le public chat
		m_panel->panel_public->TopChatWindow->WriteText(nouveauData->PublicChat);
		//actualise le private chat
		m_panel->List_PrivChat.clear(); //efface la liste
		x = m_panel->m_notebook->GetPageCount();
		for (n = m_panel->nb_tab; n < x;n++) //clear l'affichage
			m_panel->m_notebook->DeletePage(m_panel->m_notebook->GetPageCount() - 1);
		vector<wxString>::iterator VPrivChat = nouveauData->PrivChat.begin();
		vector<wxString>::iterator VPrivChatTitle = nouveauData->PrivChatTitle.begin();
		vector<int>::iterator VPrivChatId = nouveauData->PrivChatId.begin();
		for (; VPrivChat != nouveauData->PrivChat.end() 
			&& VPrivChatTitle != nouveauData->PrivChatTitle.end() 
			&& VPrivChatId != nouveauData->PrivChatId.end();
			++VPrivChat, ++VPrivChatTitle, ++VPrivChatId)
		{
			str = *VPrivChat;
			str2 = *VPrivChatTitle;
			user_id = *VPrivChatId;
			privchat = new CPrivChat(m_panel->m_notebook, str2 , x, y   ); //on cree une classe temporaire
			privchat->TopChatWindow->SetFont(wxFont(wxFontInfo(8).Family(wxFONTFAMILY_DECORATIVE).Style(wxFONTSTYLE_NORMAL).Weight(wxFONTWEIGHT_BOLD).FaceName("Unicode")));
			privchat->name = str2;
			privchat->TopChatWindow->SetValue(str);
			privchat->id_user = user_id;
			GetClientSize( &x, &y ); // on met tout a la bonne taille
			privchat->BitmapDegr->SetSize(160,0, x - 365, 24);
			privchat->splitter->SetSize(2, 26, x - 13 - WIDTH_UTILISATEUR, y - 64);
			privchat->splitter->SetSashPosition(y - 115);
			privchat->y_sash = privchat->splitter->GetSashPosition();
			privchat->SendChatButton->SetSize(x - 275, (y  - privchat->y_sash - 67) / 2 - 24, 86, 49 );
			privchat->CloseButton->SetSize(x - WIDTH_UTILISATEUR - 35, 0, 24, 24);
			privchat->ChatText->SetSize(0, 0, x - 113 - WIDTH_UTILISATEUR, y  - privchat->y_sash - 67);
			m_panel->List_PrivChat.push_back(privchat); //on enregistre la classe dans la liste
			//delete privchat;
		}
		m_panel->m_notebook->SetSelection(nouveauData->CurentSelection);
		//actualise les droits
		if (nouveauData->openuser)
			toolbar->EnableTool(CONTROLS_ADMIN, true);
		else
			toolbar->EnableTool(CONTROLS_ADMIN, false);

		if (nouveauData->broadcast)
			toolbar->EnableTool(CONTROLS_BROADCAST, true);
		else
			toolbar->EnableTool(CONTROLS_BROADCAST, false);
		if (nouveauData->sendchat)
			m_panel->panel_public->SendChatButton->Enable(true);
		else
			m_panel->panel_public->SendChatButton->Enable(false);
		if (nouveauData->downloadfile)
			m_panel->panel_transfer->ExploClient->download_button->Enable(true);
		else
			m_panel->panel_transfer->ExploClient->download_button->Enable(false);
		if (nouveauData->uploadfile)
			m_panel->panel_transfer->ExploClient->upload_button->Enable(true);
		else
			m_panel->panel_transfer->ExploClient->upload_button->Enable(false);
		if (nouveauData->deletefile)
			m_panel->panel_transfer->ExploServer->delete_button->Enable(true);
		else
			m_panel->panel_transfer->ExploServer->delete_button->Enable(false);
		if (nouveauData->createfolder)
			m_panel->panel_transfer->ExploServer->create_folder_button->Enable(true);
		else
			m_panel->panel_transfer->ExploServer->create_folder_button->Enable(false);
		if (nouveauData->downloadfile)
			m_panel->panel_transfer->ExploServer->preview_button->Enable(true);
		else
			m_panel->panel_transfer->ExploServer->preview_button->Enable(false);
		}
}
// // // // // //
void MyFrame::receive_chat(vector<wxString>::iterator i)
{
	vector<wxString>	tab_line;
	wxString			word;

	wxString msg;
	wxString line;
	wxColour login_colour;

	login_colour.Set(148,166,244);
	int n = 0;

	msg = "";
	i++;
	m_panel->panel_public->TopChatWindow->SetFont(wxFont(wxFontInfo(8).Family(wxFONTFAMILY_DECORATIVE).Style(wxFONTSTYLE_NORMAL).Weight(wxFONTWEIGHT_BOLD).FaceName("Unicode")));
	m_panel->panel_public->TopChatWindow->SetDefaultStyle(wxTextAttr(*wxBLUE));
	
	for ( ; i != tab_line_cmd.end(); ++i )
		{
			line = *i;

			if (line.at(0) == '[')
				break;

			tab_line = StringToTab(line, ":");
			for (vector<wxString>::iterator it = tab_line.begin(); it != tab_line.end(); ++it )
			{
				word = *it;
				if (n == 0)
				{
					m_panel->panel_public->TopChatWindow->SetDefaultStyle(wxTextAttr(login_colour));
					m_panel->panel_public->TopChatWindow->AppendText(word);
					m_panel->panel_public->TopChatWindow->AppendText(" says :\n");
					n = 1;
				}
				else
				{
					if (n == 1)
					{
						//word = word.AfterFirst(' ');
						word = word.AfterFirst(' ');
						n = 2;
					}
					m_panel->panel_public->TopChatWindow->SetDefaultStyle(wxTextAttr(*wxBLACK));
					m_panel->panel_public->TopChatWindow->AppendText(word);
					if (it + 1 == tab_line.end())
						m_panel->panel_public->TopChatWindow->AppendText("\n");
					else
						m_panel->panel_public->TopChatWindow->AppendText(":");
				}
				
			}
		}	
}

void MyFrame::receive_priv_chat(vector<wxString>::iterator i)
{
	//"receive_priv_chat from : NOM id : Number"

	int					id_user;
	CPrivChat			*privchat;
	wxString			msg1;
	wxString			msg2;
	wxString			line;
	vector<wxString>	tabline;
	wxColour			login_colour;

	login_colour.Set(148,166,244);

	// recuperation de l'id dans le message
	line = *i;
	tabline = StringToTab(line, " ");
	if (tabline.size() == 5)
		id_user = wxAtoi(tabline.at(4));
	else
	{
		wxMessageBox("Wrong message");
		return;
	}
	i++;

	msg1 = "" + tabline.at(2) + " says :\n";
	msg2 = "";
	for ( ; i != tab_line_cmd.end(); ++i )
		{
			line = *i;
			if (line.at(0) == '[')
				break;
			msg2 = msg2 + line + "\n";
		}	

	// on parcour tous les privchat ouverts. si on trouve l'id, on met le texte dans la fenettre correspondante. sinon, on en cree une.
	vector<CPrivChat *>::iterator it;
	privchat = NULL;
	for (it = m_panel->List_PrivChat.begin(); it != m_panel->List_PrivChat.end(); ++it)
	{
		privchat = *it;
		if(privchat->id_user == id_user)
			break;
	}
	if (it != m_panel->List_PrivChat.end())
	{
		privchat->TopChatWindow->SetFont(wxFont(wxFontInfo(8).Family(wxFONTFAMILY_DECORATIVE).Style(wxFONTSTYLE_NORMAL).Weight(wxFONTWEIGHT_BOLD).FaceName("Unicode")));
		privchat->TopChatWindow->SetDefaultStyle(wxTextAttr(login_colour));
		privchat->TopChatWindow->AppendText(msg1);
		privchat->TopChatWindow->SetDefaultStyle(wxTextAttr(*wxBLACK));
		privchat->TopChatWindow->AppendText(msg2);
	}
	else
	{
			int							x,y;

			GetClientSize( &x, &y ); // on met tout a la bonne taille
			privchat = new CPrivChat(m_panel->m_notebook, tabline.at(2) , x, y   ); //on cree une classe temporaire
			privchat->name = tabline.at(2);
			privchat->id_user = id_user;
			privchat->BitmapDegr->SetSize(160,0, x - 365, 24);
			privchat->splitter->SetSize(2, 26, x - 13 - WIDTH_UTILISATEUR, y - 64);
			privchat->splitter->SetSashPosition(y - 115);
			privchat->y_sash = privchat->splitter->GetSashPosition();
			privchat->SendChatButton->SetSize(x - 275, (y  - privchat->y_sash - 67) / 2 - 24, 86, 49 );
			privchat->CloseButton->SetSize(x - WIDTH_UTILISATEUR - 35, 0, 24, 24);
			privchat->ChatText->SetSize(0, 0, x - 113 - WIDTH_UTILISATEUR, y  - privchat->y_sash - 67);
			m_panel->List_PrivChat.push_back(privchat); //on enregistre la classe dans la liste
			privchat->TopChatWindow->SetFont(wxFont(wxFontInfo(8).Family(wxFONTFAMILY_DECORATIVE).Style(wxFONTSTYLE_NORMAL).Weight(wxFONTWEIGHT_BOLD).FaceName("Unicode")));
			privchat->TopChatWindow->SetDefaultStyle(wxTextAttr(login_colour));
			privchat->TopChatWindow->AppendText(msg1);
			privchat->TopChatWindow->SetDefaultStyle(wxTextAttr(*wxBLACK));
			privchat->TopChatWindow->AppendText(msg2);
	}
	//delete privchat;
}

void MyFrame::lost_console()
{
	int	x;
	int	n;

	if (is_connected_console == 1)
		wxMessageBox(_("Error RedCap Client connexion lost!\n"), _("Alert !"));
	is_connected_console = 0;
	SetStatusText("Connection : not connected.", 1);

	file_menu->Enable(CONTROLS_OPTIONS, FALSE);
	file_menu->Enable(CONTROLS_ADMIN, TRUE);
	connect_menu->Enable(CONTROLS_CONNECT, FALSE);
	connect_menu->Enable(CONTROLS_DISCONNECT, FALSE);
	connect_menu->Enable(CONTROLS_CONNECT_CLIENT, TRUE);
	m_panel->Enable(false);
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

	m_panel->List_users->ClearAll();
	m_panel->panel_public->TopChatWindow->Clear();
	m_panel->List_PrivChat.clear(); 
	x = m_panel->m_notebook->GetPageCount();
	for (n = m_panel->nb_tab; n < x; n++) 
		m_panel->m_notebook->DeletePage(m_panel->m_notebook->GetPageCount() - 1);
	//effacement de tous les  serv data 
	m_panel->Serv_Data.clear();
	m_panel->List_serv->Clear();
	m_panel->Current_serv_data = NULL;
	connect_menu->Enable(CONTROLS_CONNECT, FALSE);
	connect_menu->Enable(CONTROLS_CONNECT_CLIENT, TRUE);
	connect_menu->Enable(CONTROLS_DISCONNECT, FALSE);
	m_panel->panel_transfer->ExploServer->tree_list->DeleteAllItems();
	m_panel->panel_transfer->ExploClient->tree_list->DeleteAllItems();
}

void MyFrame::OnDisconnect( wxCommandEvent &event )
{
	m_panel->panel_transfer->ExploServer->tree_list->DeleteAllItems();
	m_panel->panel_transfer->ExploClient->tree_list->DeleteAllItems();
	output_message("disconnect");
}

void MyFrame::file_client_set_list(vector<wxString>::iterator i)
{
	
	int					serv;
	vector<wxString>	fichier;
	wxString			line;
	int cpt_file = 0;


	serv = m_panel->List_serv->GetSelection();
	vector<CServData * >::iterator it = m_panel->Serv_Data.begin();
	for (/*vector<CServData * >::iterator it = m_panel->Serv_Data.begin()*/; it != m_panel->Serv_Data.end(); ++it )
	{
		if(serv <= 0)
			break;
		serv --;
	}
	if (it == m_panel->Serv_Data.end())
		return;
		//wxMessageBox("Error: un file_user_list est fait sur un serveur ki existe pas...");
	i++;
	ClientRepRoot = new CRep;
	ClientRepRoot->nom = "root";

	wxTreeItemId idtree;
	wxTreeItemId RootFolder;

	m_panel->panel_transfer->ExploClient->tree_list->DeleteAllItems();

	//RootFolder = m_panel->panel_transfer->ExploClient->tree_transfer->AddRoot("Root Folder", 0, -1, NULL);


	/***************** Remplissage de la liste de repertoire ************************************/
	
	for (i++; i + 1 != tab_line_cmd.end(); ++i)
	{
		line = *i;
		if (!wxStrcmp(line.substr(0,1) , "["))
			break;
		fichier = StringToTab(line , "\t");
		if (fichier.size() == 7)
		{
			if (!wxStrncmp(fichier.at(0), "fldr", 4))
			{
				ClientRepList = new CRep();
				ClientRepList->size = fichier.at(2);
				ClientRepList->nom = fichier.at(6);
				ClientRepRoot->sous_rep.push_back(ClientRepList);
//				m_panel->panel_transfer->ExploClient->tree_transfer->AppendItem(RootFolder, ClientRepList->nom, 0, -1, NULL);
				m_panel->panel_transfer->ExploClient->tree_list->InsertItem(cpt_file, ClientRepList->nom, 0);
				m_panel->panel_transfer->ExploClient->tree_list->SetItem(cpt_file, 1, "Creator", -1);
				m_panel->panel_transfer->ExploClient->tree_list->SetItem(cpt_file, 2, ClientRepList->size, -1);
				m_panel->panel_transfer->ExploClient->tree_list->SetItem(cpt_file, 3, "Folder", -1);
				cpt_file++;
			}
			else 
			{
				ClientFileList = new CFileList();
				ClientFileList->FileType = fichier.at(0);
				ClientFileList->Creator = fichier.at(1);
				ClientFileList->size = wxStrdup(fichier.at(2));
				ClientFileList->Filename = fichier.at(6);
				ClientRepRoot->fichier.push_back(ClientFileList);
				m_panel->panel_transfer->ExploClient->tree_list->InsertItem(cpt_file, ClientFileList->Filename, 1);
				m_panel->panel_transfer->ExploClient->tree_list->SetItem(cpt_file, 1, ClientFileList->Creator, -1);
				m_panel->panel_transfer->ExploClient->tree_list->SetItem(cpt_file, 2, ClientFileList->size, -1);
				m_panel->panel_transfer->ExploClient->tree_list->SetItem(cpt_file, 3, ClientFileList->FileType, -1);
				cpt_file++;
			}
			//delete ClientRepList;
		}
	}
	//delete ClientRepRoot;
}

void MyFrame::file_set_list(vector<wxString>::iterator i)
{
	int					serv;
	vector<wxString>	fichier;
	wxString			line;
//	CRep				*RepRoot;
//	CRep 				*RepList;
//	CFileList			*FileList;

	serv = m_panel->List_serv->GetSelection();
	vector<CServData * >::iterator it = m_panel->Serv_Data.begin();
	for (/*vector<CServData * >::iterator it = m_panel->Serv_Data.begin()*/; it != m_panel->Serv_Data.end(); ++it )
	{
		if(serv <= 0)
			break;
		serv --;
	}
	if (it == m_panel->Serv_Data.end())
		return;
		//wxMessageBox("Error: un file_user_list est fait sur un serveur ki existe pas...");
	i++;
	ServRepRoot = new CRep;
	ServRepRoot->nom = CurrentRep;
	m_panel->panel_transfer->ExploServer->tree_list->DeleteAllItems();
	
	int cpt_file = 0;
	
	/************ Ajout du repertoire ".." ****************/
	
	ServRepList = new CRep();
	ServRepList->size = "0";
	ServRepList->nom = "..";
	ServRepRoot->sous_rep.push_back(ServRepList);
	m_panel->panel_transfer->ExploServer->tree_list->InsertItem(cpt_file, ServRepList->nom, 0);
	m_panel->panel_transfer->ExploServer->tree_list->SetItem(cpt_file, 1, "Creator", -1);
	m_panel->panel_transfer->ExploServer->tree_list->SetItem(cpt_file, 2, ServRepList->size, -1);
	m_panel->panel_transfer->ExploServer->tree_list->SetItem(cpt_file, 3, "Folder", -1);

	/***************** Remplissage de la liste de repertoire ************************************/

	for (cpt_file++, i++; i + 1 != tab_line_cmd.end(); i++)
	{
		line = *i;
		if (!wxStrcmp(line.substr(0,1) , "["))
			break;
		fichier = StringToTab(line , "\t");
		if (fichier.size() == 7)
		{
			if (!wxStrncmp(fichier.at(0), "fldr", 4))
			{
				wxTreeItemId TmpId;
				
				ServRepList = new CRep();
				ServRepList->size = fichier.at(2);
				ServRepList->nom = fichier.at(6);
				ServRepRoot->sous_rep.push_back(ServRepList);
				
				m_panel->panel_transfer->ExploServer->tree_list->InsertItem(cpt_file, ServRepList->nom, 0);
				m_panel->panel_transfer->ExploServer->tree_list->SetItem(cpt_file, 1, "Creator", -1);
				m_panel->panel_transfer->ExploServer->tree_list->SetItem(cpt_file, 2, ServRepList->size, -1);
				m_panel->panel_transfer->ExploServer->tree_list->SetItem(cpt_file, 3, "Folder", -1);
			}
			else
			{
				ServFileList = new CFileList();
				ServFileList->FileType = fichier.at(0);
				ServFileList->Creator = fichier.at(1);
				ServFileList->size = wxStrdup(fichier.at(2));
				ServFileList->Filename = fichier.at(6);
				ServRepRoot->fichier.push_back(ServFileList);
				m_panel->panel_transfer->ExploServer->tree_list->InsertItem(cpt_file, ServFileList->Filename, 1);
				m_panel->panel_transfer->ExploServer->tree_list->SetItem(cpt_file, 1, ServFileList->Creator, -1);
				m_panel->panel_transfer->ExploServer->tree_list->SetItem(cpt_file, 2, ServFileList->size, -1);
				m_panel->panel_transfer->ExploServer->tree_list->SetItem(cpt_file, 3, ServFileList->FileType, -1);
			}
			cpt_file++;
		}
	}
	m_panel->RefreshServerPath();
}

void MyPanel::FillFolder(wxString folder_name)/* ( wxString folder, wxTreeItemId id)*/
{
	panel_transfer->ExploServer->tree_list->DeleteAllItems();
	//panel_transfer->ExploClient->tree_list->DeleteAllFileList();
	MainFrame->CurrentRep = folder_name;
	MainFrame->CurrentId = wxTreeItemId();
	MainFrame->output_message("cd \"" + folder_name + "\"");
	//MainFrame->output_message("pwd");
	MainFrame->output_message("ls");
}

void MyFrame::FillSousRep(wxString CurrentRep, wxTreeItemId CurrentId)
{
	int cpt_file = 0;
	vector<wxString>	fichier;
	wxString			line;

//	tab_line_cmd_last = tab_line_cmd;
	for ( vector<wxString>::iterator i = tab_line_cmd.begin(); i != tab_line_cmd.end(); ++i )
	{
		line = *i;
		if (!wxStrcmp(line.substr(0,1) , "["))
			break;
		
		fichier = StringToTab(line , "\t");
		if (fichier.size() == 7)
		{
			if (!wxStrncmp(fichier.at(0), "fldr", 4))
			{
				wxTreeItemId TmpId;

				CurrentServRepList = new CRep();
				CurrentServRepList->size = fichier.at(2);
				CurrentServRepList->nom = fichier.at(6);
				CurrentServRepRoot->sous_rep.push_back(CurrentServRepList);
				
				
				// Insertion dans le panel de Gauche
				//TmpId = m_panel->panel_transfer->ExploServer->tree_transfer->AppendItem(CurrentId, 
				//													CurrentServRepList->nom, 0, -1, NULL);
				//m_panel->panel_transfer->ExploServer->tree_transfer->AppendItem(TmpId, 
				//														"TOTO N'est Pas LA PAR HASARD", 0, -1, NULL);

				// Insertion dans le panel de droite
				m_panel->panel_transfer->ExploServer->tree_list->InsertItem(cpt_file, CurrentServRepList->nom, 0);
				m_panel->panel_transfer->ExploServer->tree_list->SetItem(cpt_file, 1, "Creator", -1);
				m_panel->panel_transfer->ExploServer->tree_list->SetItem(cpt_file, 2, CurrentServRepList->size, -1);
				m_panel->panel_transfer->ExploServer->tree_list->SetItem(cpt_file, 3, "Folder", -1);
				//FillFolder(ServRepList->nom, CurrentId); // On va remplir le rep : ServRepList->nom
			}
			else
			{
				CurrentServFileList = new CFileList();
				CurrentServFileList->FileType = fichier.at(0);
				CurrentServFileList->Creator = fichier.at(1);
				CurrentServFileList->size = wxStrdup(fichier.at(2));
				CurrentServFileList->Filename = fichier.at(6);
				CurrentServRepRoot->fichier.push_back(ServFileList);
				m_panel->panel_transfer->ExploServer->tree_list->InsertItem(cpt_file, CurrentServFileList->Filename, 1);
				m_panel->panel_transfer->ExploServer->tree_list->SetItem(cpt_file, 1, CurrentServFileList->Creator, -1);
				m_panel->panel_transfer->ExploServer->tree_list->SetItem(cpt_file, 2, CurrentServFileList->size, -1);
				m_panel->panel_transfer->ExploServer->tree_list->SetItem(cpt_file, 3, CurrentServFileList->FileType, -1);
			}
			cpt_file++;
			//delete ServFileList;
		}
	}
	tab_line_cmd = tab_line_cmd_last;
}


void MyFrame::get_user(vector<wxString>::iterator i)
{
	wxString			line;
	wxString			UserName;
	for (i++; i != tab_line_cmd.end(); ++i )
	{
	//wxMessageBox(line);
		line = *i;
		if (!wxStrcmp(line.substr(0,1) , "["))
			break;
		if (!wxStrcmp(line, "user_name:"))
		{          
			line = *++i;
			m_admin->UserName->Clear();
			m_admin->UserName->AppendText(line);
		}
		if (!wxStrcmp(line, "user_login:"))
		{ 
			line = *++i;
			m_admin->login->Clear();
			m_admin->login->AppendText(line);
		}
		//if (!wxStrcmp(line, "user_password:"))
		//{ 
	//		line = *++i;
	//		m_admin->password->Clear();
//			m_admin->password->AppendText(line);
//		}
		if (!wxStrcmp(line, "Delete file:0")){ m_admin->deletefile->SetValue(FALSE);}
		if (!wxStrcmp(line, "Upload file:0")){ m_admin->uploadfile->SetValue(FALSE);}
		if (!wxStrcmp(line, "Download file:0")){ m_admin->downloadfile->SetValue(FALSE);}
		if (!wxStrcmp(line, "Rename file:0")){ m_admin->renamefile->SetValue(FALSE);}
		if (!wxStrcmp(line, "Move file:0")){ m_admin->movefile->SetValue(FALSE);}
		if (!wxStrcmp(line, "Create folder:0")){ m_admin->createfolder->SetValue(FALSE);}
		if (!wxStrcmp(line, "Delete folder:0")){ m_admin->deletefolder->SetValue(FALSE);}
		if (!wxStrcmp(line, "Rename folder:0")){ m_admin->renamefolder->SetValue(FALSE);}
		if (!wxStrcmp(line, "Move folder:0")){ m_admin->movefolder->SetValue(FALSE);}
		if (!wxStrcmp(line, "Read chat:0")){ m_admin->readchat->SetValue(FALSE);}
		if (!wxStrcmp(line, "Send chat:0")){ m_admin->sendchat->SetValue(FALSE);}
		if (!wxStrcmp(line, "Open chat:0")){ m_admin->openchat->SetValue(FALSE);}
		if (!wxStrcmp(line, "Close chat:0")){ m_admin->closechat->SetValue(FALSE);}
		if (!wxStrcmp(line, "Show in list:0")){ m_admin->showinlist->SetValue(FALSE);}
		if (!wxStrcmp(line, "Create user:0")){ m_admin->createuser->SetValue(FALSE);}
		if (!wxStrcmp(line, "Delete user:0")){ m_admin->deleteuser->SetValue(FALSE);}
		if (!wxStrcmp(line, "Open user:0")){ m_admin->openuser->SetValue(FALSE);}
		if (!wxStrcmp(line, "Modify user:0")){ m_admin->modifyuser->SetValue(FALSE);}
		if (!wxStrcmp(line, "Change own password:0")){ m_admin->changeownpassword->SetValue(FALSE);}
		if (!wxStrcmp(line, "Send private message:0")){ m_admin->sendprivatemessage->SetValue(FALSE);}
		if (!wxStrcmp(line, "Upload anywhere:0")){ m_admin->uploadanywhere->SetValue(FALSE);}
		if (!wxStrcmp(line, "Any name:0")){ m_admin->anyname->SetValue(FALSE);}
		if (!wxStrcmp(line, "No agreement:0")){ m_admin->noagreement->SetValue(FALSE);}
		if (!wxStrcmp(line, "Set file comment:0")){ m_admin->getfilecomment->SetValue(FALSE);}
		if (!wxStrcmp(line, "Set folder comment:0")){ m_admin->setfoldercomment->SetValue(FALSE);}
		if (!wxStrcmp(line, "View drop boxes:0")){ m_admin->viewdropboxes->SetValue(FALSE);}
		if (!wxStrcmp(line, "Make alias:0")){ m_admin->makealias->SetValue(FALSE);}
		if (!wxStrcmp(line, "Broadcast:0")){ m_admin->broadcast->SetValue(FALSE);}
		/*
		//if (!wxStrcmp(line, "News read article:0")){ m_admin->newsreadarticle->SetValue(FALSE);}
		//if (!wxStrcmp(line, "News post article:0")){ m_admin->newspostarticle->SetValue(FALSE);}
		if (!wxStrcmp(line, "Disconnect user:0")){ m_admin->disconnectuser->SetValue(FALSE);}
		if (!wxStrcmp(line, "Cannot be disconnect:0")){ m_admin->cannotbedisconnect->SetValue(FALSE);}
		if (!wxStrcmp(line, "Get client info:0")){ m_admin->getclientinfo->SetValue(FALSE);}
		if (!wxStrcmp(line, "News delete article:0")){ m_admin->newsdeletearticle->SetValue(FALSE);}
		if (!wxStrcmp(line, "News create category:0")){ m_admin->newscreatecategory->SetValue(FALSE);}
		if (!wxStrcmp(line, "News delete category:0")){ m_admin->newsdeletecategory->SetValue(FALSE);}
		if (!wxStrcmp(line, "News create folder:0")){ m_admin->newscreatefolder->SetValue(FALSE);}
		if (!wxStrcmp(line, "News delete folder:0")){ m_admin->newsdeletefolder->SetValue(FALSE);}*/

		if (!wxStrcmp(line, "Delete file:1")){ m_admin->deletefile->SetValue(TRUE);}
		if (!wxStrcmp(line, "Upload file:1")){ m_admin->uploadfile->SetValue(TRUE);}
		if (!wxStrcmp(line, "Download file:1")){ m_admin->downloadfile->SetValue(TRUE);}
		if (!wxStrcmp(line, "Rename file:1")){ m_admin->renamefile->SetValue(TRUE);}
		if (!wxStrcmp(line, "Move file:1")){ m_admin->movefile->SetValue(TRUE);}
		if (!wxStrcmp(line, "Create folder:1")){ m_admin->createfolder->SetValue(TRUE);}
		if (!wxStrcmp(line, "Delete folder:1")){ m_admin->deletefolder->SetValue(TRUE);}
		if (!wxStrcmp(line, "Rename folder:1")){ m_admin->renamefolder->SetValue(TRUE);}
		if (!wxStrcmp(line, "Move folder:1")){ m_admin->movefolder->SetValue(TRUE);}
		if (!wxStrcmp(line, "Read chat:1")){ m_admin->readchat->SetValue(TRUE);}
		if (!wxStrcmp(line, "Send chat:1")){ m_admin->sendchat->SetValue(TRUE);}
		if (!wxStrcmp(line, "Open chat:1")){ m_admin->openchat->SetValue(TRUE);}
		if (!wxStrcmp(line, "Close chat:1")){ m_admin->closechat->SetValue(TRUE);}
		if (!wxStrcmp(line, "Show in list:1")){ m_admin->showinlist->SetValue(TRUE);}
		if (!wxStrcmp(line, "Create user:1")){ m_admin->createuser->SetValue(TRUE);}
		if (!wxStrcmp(line, "Delete user:1")){ m_admin->deleteuser->SetValue(TRUE);}
		if (!wxStrcmp(line, "Open user:1")){ m_admin->openuser->SetValue(TRUE);}
		if (!wxStrcmp(line, "Modify user:1")){ m_admin->modifyuser->SetValue(TRUE);}
		if (!wxStrcmp(line, "Change own password:1")){ m_admin->changeownpassword->SetValue(TRUE);}
		if (!wxStrcmp(line, "Send private message:1")){ m_admin->sendprivatemessage->SetValue(TRUE);}
		if (!wxStrcmp(line, "Upload anywhere:1")){ m_admin->uploadanywhere->SetValue(TRUE);}
		if (!wxStrcmp(line, "Any name:1")){ m_admin->anyname->SetValue(TRUE);}
		if (!wxStrcmp(line, "No agreement:1")){ m_admin->noagreement->SetValue(TRUE);}
		if (!wxStrcmp(line, "Set file comment:1")){ m_admin->getfilecomment->SetValue(TRUE);}
		if (!wxStrcmp(line, "Set folder comment:1")){ m_admin->setfoldercomment->SetValue(TRUE);}
		if (!wxStrcmp(line, "View drop boxes:1")){ m_admin->viewdropboxes->SetValue(TRUE);}
		if (!wxStrcmp(line, "Make alias:1")){ m_admin->makealias->SetValue(TRUE);}
		if (!wxStrcmp(line, "Broadcast:1")){ m_admin->broadcast->SetValue(TRUE);}
		//if (!wxStrcmp(line, "Disconnect user:1")){ m_admin->disconnectuser->SetValue(TRUE);}
		//if (!wxStrcmp(line, "Cannot be disconnect:1")){ m_admin->cannotbedisconnect->SetValue(TRUE);}
		//if (!wxStrcmp(line, "Get client info:1")){ m_admin->getclientinfo->SetValue(TRUE);}	
		//if (!wxStrcmp(line, "News read article:1")){ m_admin->newsreadarticle->SetValue(TRUE);}
		//if (!wxStrcmp(line, "News post article:1")){ m_admin->newspostarticle->SetValue(TRUE);}
		//if (!wxStrcmp(line, "News delete article:1")){ m_admin->newsdeletearticle->SetValue(TRUE);}
		//if (!wxStrcmp(line, "News create category:1")){ m_admin->newscreatecategory->SetValue(TRUE);}
		//if (!wxStrcmp(line, "News delete category:1")){ m_admin->newsdeletecategory->SetValue(TRUE);}
		//if (!wxStrcmp(line, "News create folder:1")){ m_admin->newscreatefolder->SetValue(TRUE);}
		//if (!wxStrcmp(line, "News delete folder:1")){ m_admin->newsdeletefolder->SetValue(TRUE);}*/
	}
		//wxMessageBox("lolo");

}

void MyFrame::get_login_list(vector<wxString>::iterator i)
{
	wxString			line;
	
	m_admin->account_list.clear();
	for (i++; i != tab_line_cmd.end(); ++i )
	{
		line = *i;
		if (!wxStrcmp(line.substr(0,1) , "["))
			break;
		m_admin->account_list.push_back(line.AfterFirst('\t'));
	}
	m_admin->FillAcountList();
}

void MyFrame::user_set_list(vector<wxString>::iterator i)
{
	vector<wxString>	utilisateur;
	wxString			line;
	CServData			*tmp;
	CUser				*nuser;
	wxString			status;
	vector<wxString>	FlagLst;
	
	m_panel->List_users->ClearAll();
	tmp = m_panel->Current_serv_data;
	i++;
	tmp->Users.clear();
	for (i++; i != tab_line_cmd.end(); ++i )
	{
		line = *i;
		if (!wxStrcmp(line.substr(0,1) , "["))
			break;
		utilisateur = StringToTab(line , "\t");

		/*	
	user_list:
id	icon	flag			size	name
		Rpm  Rpc  Type Away
12	2001	0 0 3 0		6	  RedCap
		*/

		if (utilisateur.size() >= 4)
		{
			nuser = new CUser;
			nuser->name = utilisateur.at(4);
			nuser->id = wxAtoi(utilisateur.at(0));
			nuser->icon = utilisateur.at(1);
		
			/* on recompose le nom de l'icone ki sera afficher en fct du statut user */
			
			status = utilisateur.at(2);
			FlagLst = StringToTab(status, " ");

			if (FlagLst.size() != 4)
			{
				wxMessageBox("An error occured in user_set_list()");
				break;
			}
			else
			{
				nuser->num_icon = 4;
				if (!strncmp(FlagLst.at(2), "1", 1))
					nuser->num_icon = 8;
				else if (!strncmp(FlagLst.at(2), "2", 1))
					nuser->num_icon = 12;
				else if (!strncmp(FlagLst.at(2), "3", 1))
					nuser->num_icon = 16;
				if (FlagLst.at(3) == '1')
					nuser->num_icon += 1;
				if (FlagLst.at(1) == '1')
					nuser->num_icon += 2;
				//wxMessageBox("num icon :");
				//IntToBox(num_icon);
			}
			
			/***********************************************************************/

			tmp->Users.push_back(nuser);
			//wxMessageBox("USER = " + nuser->name);
			m_panel->List_users->InsertItem(m_panel->List_users->GetItemCount(), nuser->name, nuser->num_icon);
		}
		//delete nuser;
	}
	//Affstruct();
}

void MyFrame::user_access(vector<wxString>::iterator i)
{
	vector<wxString>	utilisateur;
	wxString			line;
	CServData			*tmp;

	tmp = m_panel->Current_serv_data;
	i++;
	for (i++; i != tab_line_cmd.end(); ++i )
	{
		line = *i;
		if (!wxStrcmp(line.substr(0,1) , "["))
			break;
		if (!wxStrcmp(line, "Delete file:0")){ tmp->deletefile = false;}
		if (!wxStrcmp(line, "Upload file:0")){ tmp->uploadfile = false;}
		if (!wxStrcmp(line, "Download file:0")){ tmp->downloadfile = false;}
		if (!wxStrcmp(line, "Rename file:0")){ tmp->renamefile = false;}
		if (!wxStrcmp(line, "Move file:0")){ tmp->movefile = false;}
		if (!wxStrcmp(line, "Create folder:0")){ tmp->createfolder = false;}
		if (!wxStrcmp(line, "Delete folder:0")){ tmp->deletefolder = false;}
		if (!wxStrcmp(line, "Rename folder:0")){ tmp->renamefolder = false;}
		if (!wxStrcmp(line, "Move folder:0")){ tmp->movefolder = false;}
		if (!wxStrcmp(line, "Read chat:0")){ tmp->readchat = false;}
		if (!wxStrcmp(line, "Send chat:0")){ tmp->sendchat = false;}
		if (!wxStrcmp(line, "Open chat:0")){ tmp->openchat = false;}
		if (!wxStrcmp(line, "Close chat:0")){ tmp->closechat = false;}
		if (!wxStrcmp(line, "Show in list:0")){ tmp->showinlist = false;}
		if (!wxStrcmp(line, "Create user:0")){ tmp->createuser = false;}
		if (!wxStrcmp(line, "Delete user:0")){ tmp->deleteuser = false;}
		if (!wxStrcmp(line, "Open user:0")){ tmp->openuser = false;}
		if (!wxStrcmp(line, "Modify user:0")){ tmp->modifyuser = false;}
		if (!wxStrcmp(line, "Change own password:0")){ tmp->changeownpassword = false;}
		if (!wxStrcmp(line, "Send private message:0")){ tmp->sendprivatemessage = false;}
		if (!wxStrcmp(line, "News read article:0")){ tmp->newsreadarticle = false;}
		if (!wxStrcmp(line, "News post article:0")){ tmp->newspostarticle = false;}
		if (!wxStrcmp(line, "Disconnect user:0")){ tmp->disconnectuser = false;}
		if (!wxStrcmp(line, "Cannot be disconnect:0")){ tmp->cannotbedisconnect = false;}
		if (!wxStrcmp(line, "Get client info:0")){ tmp->getclientinfo = false;}
		if (!wxStrcmp(line, "Upload anywhere:0")){ tmp->uploadanywhere = false;}
		if (!wxStrcmp(line, "Any name:0")){ tmp->anyname = false;}
		if (!wxStrcmp(line, "No agreement:0")){ tmp->noagreement = false;}
		if (!wxStrcmp(line, "Set file comment:0")){ tmp->getfilecomment = false;}
		if (!wxStrcmp(line, "Set folder comment:0")){ tmp->setfoldercomment = false;}
		if (!wxStrcmp(line, "View drop boxes:0")){ tmp->viewdropboxes = false;}
		if (!wxStrcmp(line, "Make alias:0")){ tmp->makealias = false;}
		if (!wxStrcmp(line, "Broadcast:0")){ tmp->broadcast = false;}
		if (!wxStrcmp(line, "News delete article:0")){ tmp->newsdeletearticle = false;}
		if (!wxStrcmp(line, "News create category:0")){ tmp->newscreatecategory = false;}
		if (!wxStrcmp(line, "News delete category:0")){ tmp->newsdeletecategory = false;}
		if (!wxStrcmp(line, "News create folder:0")){ tmp->newscreatefolder = false;}
		if (!wxStrcmp(line, "News delete folder:0")){ tmp->newsdeletefolder = false;}

		if (!wxStrcmp(line, "Delete file:1")){ tmp->deletefile = true;}
		if (!wxStrcmp(line, "Upload file:1")){ tmp->uploadfile = true;}
		if (!wxStrcmp(line, "Download file:1")){ tmp->downloadfile = true;}
		if (!wxStrcmp(line, "Rename file:1")){ tmp->renamefile = true;}
		if (!wxStrcmp(line, "Move file:1")){ tmp->movefile = true;}
		if (!wxStrcmp(line, "Create folder:1")){ tmp->createfolder = true;}
		if (!wxStrcmp(line, "Delete folder:1")){ tmp->deletefolder = true;}
		if (!wxStrcmp(line, "Rename folder:1")){ tmp->renamefolder = true;}
		if (!wxStrcmp(line, "Move folder:1")){ tmp->movefolder = true;}
		if (!wxStrcmp(line, "Read chat:1")){ tmp->readchat = true;}
		if (!wxStrcmp(line, "Send chat:1")){ tmp->sendchat = true;}
		if (!wxStrcmp(line, "Open chat:1")){ tmp->openchat = true;}
		if (!wxStrcmp(line, "Close chat:1")){ tmp->closechat = true;}
		if (!wxStrcmp(line, "Show in list:1")){ tmp->showinlist = true;}
		if (!wxStrcmp(line, "Create user:1")){ tmp->createuser = true;}
		if (!wxStrcmp(line, "Delete user:1")){ tmp->deleteuser = true;}
		if (!wxStrcmp(line, "Open user:1")){ tmp->openuser = true;}
		if (!wxStrcmp(line, "Modify user:1")){ tmp->modifyuser = true;}
		if (!wxStrcmp(line, "Change own password:1")){ tmp->changeownpassword = true;}
		if (!wxStrcmp(line, "Send private message:1")){ tmp->sendprivatemessage = true;}
		if (!wxStrcmp(line, "News read article:1")){ tmp->newsreadarticle = true;}
		if (!wxStrcmp(line, "News post article:1")){ tmp->newspostarticle = true;}
		if (!wxStrcmp(line, "Disconnect user:1")){ tmp->disconnectuser = true;}
		if (!wxStrcmp(line, "Cannot be disconnect:1")){ tmp->cannotbedisconnect = true;}
		if (!wxStrcmp(line, "Get client info:1")){ tmp->getclientinfo = true;}
		if (!wxStrcmp(line, "Upload anywhere:1")){ tmp->uploadanywhere = true;}
		if (!wxStrcmp(line, "Any name:1")){ tmp->anyname = true;}
		if (!wxStrcmp(line, "No agreement:1")){ tmp->noagreement = true;}
		if (!wxStrcmp(line, "Set file comment:1")){ tmp->getfilecomment = true;}
		if (!wxStrcmp(line, "Set folder comment:1")){ tmp->setfoldercomment = true;}
		if (!wxStrcmp(line, "View drop boxes:1")){ tmp->viewdropboxes = true;}
		if (!wxStrcmp(line, "Make alias:1")){ tmp->makealias = true;}
		if (!wxStrcmp(line, "Broadcast:1")){ tmp->broadcast = true;}
		if (!wxStrcmp(line, "News delete article:1")){ tmp->newsdeletearticle = true;}
		if (!wxStrcmp(line, "News create category:1")){ tmp->newscreatecategory = true;}
		if (!wxStrcmp(line, "News delete category:1")){ tmp->newsdeletecategory = true;}
		if (!wxStrcmp(line, "News create folder:1")){ tmp->newscreatefolder = true;}
		if (!wxStrcmp(line, "News delete folder:1")){ tmp->newsdeletefolder = true;}
	}

	if (tmp->openuser)
		toolbar->EnableTool(CONTROLS_ADMIN, true);
	else
		toolbar->EnableTool(CONTROLS_ADMIN, false);

	if (tmp->broadcast)
		toolbar->EnableTool(CONTROLS_BROADCAST, true);
	else
		toolbar->EnableTool(CONTROLS_BROADCAST, false);

	if (tmp->sendchat)
		m_panel->panel_public->SendChatButton->Enable(true);
	else
		m_panel->panel_public->SendChatButton->Enable(false);

	if (tmp->downloadfile)
		m_panel->panel_transfer->ExploClient->download_button->Enable(true);
	else
		m_panel->panel_transfer->ExploClient->download_button->Enable(false);

	if (tmp->uploadfile)
		m_panel->panel_transfer->ExploClient->upload_button->Enable(true);
	else
		m_panel->panel_transfer->ExploClient->upload_button->Enable(false);

	if (tmp->deletefile)
		m_panel->panel_transfer->ExploServer->delete_button->Enable(true);
	else
		m_panel->panel_transfer->ExploServer->delete_button->Enable(false);

	if (tmp->createfolder)
		m_panel->panel_transfer->ExploServer->create_folder_button->Enable(true);
	else
		m_panel->panel_transfer->ExploServer->create_folder_button->Enable(false);

	if (tmp->downloadfile)
		m_panel->panel_transfer->ExploServer->preview_button->Enable(true);
	else
		m_panel->panel_transfer->ExploServer->preview_button->Enable(false);

	//*****************  on demande les fichiers si on est en train de se logger
	if (login_in_progress)
	{
		output_message("pwd");
		output_message("ls");
		output_message("lls");
		//output_message("get_user_list");
		login_in_progress = false;
		output_message("server_list");
	}
}

void MyFrame::user_change(vector<wxString>::iterator i)
{
	output_message("get_user_list");
}


void MyFrame::agree()
{
	wxString agri;
	wxString line;
	int n = 0;

	agri = "";

	for ( vector<wxString>::iterator i = tab_line_cmd.begin(); i != tab_line_cmd.end(); i++ )
		{
			line = *i;

			if (!wxStrcmp(line, "Agreement:"))
			{ n = 1; i++; line = *i;}

			if (!wxStrcmp(line, "--------------------------------------------------") && n == 1)
			{ n = 2; i++; line = *i;}

			if (!wxStrcmp(line, "--------------------------------------------------") && n == 2)
				break;
 
			if (n == 2)
				agri = agri + line + "\n";
		}
	CAgree  dialog(this, -1, wxT("Server's agreement"), wxDefaultPosition, agri);
	dialog.CentreOnParent();
	dialog.ShowModal();
}

void MyFrame::AffFileInfo(vector<wxString>::iterator i)
{
	wxString			line;
	vector<wxString>	tabline;
	wxString			file_name;
	wxString			other_info;
	/* on recupere les informations du client */		
	i++;
	for (other_info = "", i++; i != tab_line_cmd.end(); ++i )
	{
		line = *i;
		if (!wxStrcmp(line.substr(0,1) , "["))
			break;
		tabline = StringToTab(line , "\t");
		if (tabline.size() == 2)
		{
			if (!wxStrncmp(tabline.at(0), "File name:", 10))
				file_name = tabline.at(1);
			else
				other_info += line + "\n";
		}
		else
			wxMessageBox("File Info error");
	}

	FileInfo dialog_info(this, -1, file_name, other_info, wxT("Informations sur le fichier"), wxDefaultPosition);
	dialog_info.CentreOnParent();
    dialog_info.ShowModal();
}

void MyFrame::broadcast_msg(vector<wxString>::iterator i)
{
	wxString line;
	vector<wxString>	tabline;
	wxString			broadcast_sender;

	line = *i;
	tabline = StringToTab(line, " ");
	
	if(tabline.size() != 3)
	{
		wxMessageBox("wrong broadcast");
		return;
	}
	broadcast_sender = ".:: BROADCAST Message From " + tabline.at(2) + " ::.";
	Broadcast_String = "";
	for ( ++i; i != tab_line_cmd.end(); ++i )
		{
			line = *i;
			if (line.at(0) == '[')
				break;	
			Broadcast_String = Broadcast_String + line + "\n";
		}
	BroadcastFlag = 1;
	BroadcastDialog broadcast_box(this, -1, broadcast_sender, wxDefaultPosition);
	broadcast_box.CentreOnParent();
	broadcast_box.ShowModal();
}

void MyFrame::receive_server_msg(vector<wxString>::iterator i)
{
	wxString line;
	vector<wxString>	tabline;
	wxString			broadcast_sender;

	line = *i;	
	broadcast_sender = ".:: SERVER Message From Administrator ::.";
	Broadcast_String = "";
	for (; i != tab_line_cmd.end(); ++i )
		{
			line = *i;
			if (line.at(0) == '[')
				break;	
			tabline = StringToTab(line, ":");
			if (tabline.size() >= 1)
				Broadcast_String = Broadcast_String + tabline.at(1) + "";
		}
	BroadcastFlag = 1;
	BroadcastDialog broadcast_box(this, -1, broadcast_sender, wxDefaultPosition);
	broadcast_box.CentreOnParent();
	broadcast_box.ShowModal();
}

void MyFrame::task_list(vector<wxString>::iterator i)
{
	wxString	line;
	vector<wxString>	selected;
	vector<wxString>	fichier;
	long		nb_task;
	wxListItem	info;

	nb_task = m_panel->panel_download->tree_list_download->GetItemCount();
	if (nb_task != 0)
	{
		long item = -1;
		for (;;)
		{
			item = m_panel->panel_download->tree_list_download->GetNextItem(item,
																			wxLIST_NEXT_ALL,
																			wxLIST_STATE_SELECTED);
			if (item == -1)
				break;
			info.m_itemId = item;
			info.m_col = 7;
			info.m_mask = wxLIST_MASK_TEXT;
			m_panel->panel_download->tree_list_download->GetItem(info);
			selected.push_back(info.m_text.c_str());
		}
		m_panel->panel_download->tree_list_download->DeleteAllItems();	
	}

	i++;		
	for (i++; i != tab_line_cmd.end(); ++i )
	{
		line = *i;
		if (!wxStrcmp(line.substr(0,1) , "["))
			break;
		fichier = StringToTab(line , "\t");
		m_panel->panel_download->tree_list_download->InsertItem(0, fichier.at(0), -1);
		m_panel->panel_download->tree_list_download->SetItem(0, 1, fichier.at(7), -1);
		m_panel->panel_download->tree_list_download->SetItem(0, 3, fichier.at(2), -1);
		m_panel->panel_download->tree_list_download->SetItem(0, 2, fichier.at(3), -1);
		m_panel->panel_download->tree_list_download->SetItem(0, 4, fichier.at(5), -1);
		m_panel->panel_download->tree_list_download->SetItem(0, 5, fichier.at(3), -1);
		m_panel->panel_download->tree_list_download->SetItem(0, 6, fichier.at(6), -1);
		m_panel->panel_download->tree_list_download->SetItem(0, 7, fichier.at(8), -1);
		wxString tmp;
		for (vector<wxString>::iterator j = selected.begin(); j != selected.end();)
		{
			tmp = *j;
			if (!wxStrcmp(tmp, fichier.at(8)))
			{
                m_panel->panel_download->tree_list_download->SetItemState(0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
				selected.erase(j);
			}
			else
				j++;
		}
	}
	selected.clear();
	if (m_panel->Selected_page == 2)
	{
		wxSleep(1);
		output_message("task");
	}
}
