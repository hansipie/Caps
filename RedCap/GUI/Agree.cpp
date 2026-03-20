#include "general.h"
#define TEXT 099
#define TEXT_HOST -50
#define TEXT_PORT -50
#define BUTTON_CONNECT_CLIENT -50



CAgree::CAgree(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, wxString agreement_txt)
:wxDialog(parent, id, title, pos, wxSize(500, 400))
{
	//wxString agreement_txt;

	//agreement_txt = "coucouco\nupdiuf dfhv kjfh klbjhfckjbh flkbh lkjvn bkjlv lkjzbnkjvz cnkbljn vzlkjbn kljv\nznc klbjn vzkjlcn bkljvnz clkjbn lkvjzcn bkljncvzlbnkljvzcn kbljn vzcbn lkjznkfljdnbilunf dzljknblkjznklbjn cvlb lkjnzklbjn kljn bjkzvncbkljnz vkjnb kljznvkljn bkljzcvn \nkbjlnzvnblkjzcvnkbljnzlkjbn kljzvn bkj vzb v  nzcj vblkj zkvjn bkljzvn bnlzkvjnc blk nzklvcbn lzkjvn \nklbjn zvlkjn bkljvzn klbjn klzvjnc blkn zklvjn blkjvnz lkbjn zlkvjn blkjvznblkj nvkljnblkjznv kj";

	MainFrame = (MyFrame *)parent;
    
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxTextCtrl *item1 = new wxTextCtrl( this, ID_AGREEMENT, wxT(agreement_txt), wxDefaultPosition, wxSize(400,230), wxTE_MULTILINE|wxTE_READONLY|wxVSCROLL );
    item0->Add( item1, 0, wxALIGN_CENTER|wxALL, 5 );

    wxBoxSizer *item2 = new wxBoxSizer( wxHORIZONTAL );

    wxButton *item3 = new wxButton( this, ID_AGREE, wxT("Agree"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->SetDefault();
    item2->Add( item3, 0, wxALIGN_CENTER|wxALL, 5 );

    wxButton *item4 = new wxButton( this, ID_DISAGREE, wxT("Disagree"), wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item4, 0, wxALIGN_CENTER|wxALL, 5 );

    item0->Add( item2, 0, wxALIGN_CENTER|wxALL, 5 );

    this->SetSizer( item0 );
    item0->SetSizeHints( this );

    SetAutoLayout(TRUE);
	Layout();
}

void CAgree::OnCancel(void)
{
	MainFrame->output_message("disagree");
	Close(TRUE);
}

void CAgree::OnOk(void)
{
	MainFrame->output_message("agree");
	if (!(MainFrame->connect_opt_ag))
		Close(TRUE);
	MainFrame->m_panel->List_users->DeleteAllItems();
	MainFrame->m_panel->panel_transfer->ExploServer->tree_list->DeleteAllItems();
	MainFrame->m_panel->panel_transfer->ExploClient->tree_list->DeleteAllItems();
	MainFrame->output_message("list_server");
	//MainFrame->output_message("lls");
}

CAgree::~CAgree(void)
{
}

BEGIN_EVENT_TABLE(CAgree, wxDialog)
EVT_BUTTON(ID_DISAGREE, CAgree::OnCancel)
EVT_BUTTON(ID_AGREE, CAgree::OnOk)
END_EVENT_TABLE()
