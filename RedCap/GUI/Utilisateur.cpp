#include "general.h"


void MyPanel::OnDbleClickUtilisateur()
{
	    int x = 0;
		int y = 0;
		long item, item2;
		CServData					*dataserv;
		CUser						*user = NULL;
		
		dataserv = Current_serv_data;
		if (!Current_serv_data /*|| Current_serv_data->sendprivatemessage*/) return;

		// on met item a la bonne valeur
		item = List_users->GetNextItem(-1, wxLIST_NEXT_ALL,
                                        wxLIST_STATE_SELECTED);

		// on parcours tous les users de data serv 
		// si on selectionne le 2 eme utilisateur de la liste, cette boucle s'arretera sur le 2eme user de dataserv
		item2 = item;
		for ( vector<CUser *>::iterator i = dataserv->Users.begin(); i != dataserv->Users.end(); ++i)
		{
			user = *i;
			if(item2 <= 0)
				break;
			item2 = item2 - 1;
		}

		//on parcour tous les privchat ouverts
		// si il y a un chat ouvert qui a le meme ID que le user, alrs c que la conversation etait deja entamee
		for ( vector< CPrivChat * >::iterator i = List_PrivChat.begin(); i != List_PrivChat.end(); ++i )
		{			
			CPrivChat  *tmp = *i;

			if (tmp->id_user == user->id)
			{
				x = 1;
				m_notebook->SetSelection(y + nb_tab);  //3 par ce ke y a 3 onglets au debut ki sont pas des chats privés
			}
			y++;
		}
		y = 0;
		if (!x)
		{
		//si on corrige cette partie, corriger aussi dans engine.cpp

		GetClientSize( &x, &y ); // on met tout a la bonne taille
		CPrivChat  *privchat = new CPrivChat(m_notebook, _T(List_users->GetItemText(item).c_str()) , x, y   ); //on cree une classe temporaire
		privchat->name = wxStrdup(List_users->GetItemText(item).c_str());
		privchat->id_user = user->id;
#ifdef __WIN32__		
privchat->BitmapDegr->SetSize(160,0, x - 365, 24);
#endif
		privchat->splitter->SetSize(2, 26, x - 13 - WIDTH_UTILISATEUR, y - 64);
		privchat->splitter->SetSashPosition(y - 115);
		privchat->y_sash = privchat->splitter->GetSashPosition();
		privchat->SendChatButton->SetSize(x - 275, (y  - privchat->y_sash - 67) / 2 - 24, 86, 49 );
		privchat->CloseButton->SetSize(x - WIDTH_UTILISATEUR - 35, 0, 24, 24);
		privchat->ChatText->SetSize(0, 0, x - 113 - WIDTH_UTILISATEUR, y  - privchat->y_sash - 67);
		List_PrivChat.push_back(privchat); //on enregistre la classe dans la liste
		}
}
