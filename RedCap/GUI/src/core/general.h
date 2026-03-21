// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#if !defined( __WXMSW__ ) || defined( __WIN95__ )
#include "wx/spinbutt.h"
#endif
#include "wx/tglbtn.h"
#include "wx/notebook.h"
#include "wx/imaglist.h"
#include "wx/artprov.h"
#include "wx/splitter.h"
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/splash.h>
#include <wx/progdlg.h>
#include <wx/event.h>



#if wxUSE_TOOLTIPS
    #include "wx/tooltip.h"
#endif

#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #define USE_XPM
#endif

#include "wx/progdlg.h"
#include <wx/dialog.h>
//socket include
#include "wx/socket.h"
#include "wx/url.h"
#include "wx/wfstream.h"

//vectors
#include <iostream>
#include <vector>
using namespace std;

//Private include
#include "MyApp.h"
#include "MyFrame.h"
#include "MyPanel.h"
#include "Connexion.h"
#include "Connexion_client.h"
#include "ServData.h"
#include "Options.h"
#include "Administration.h"
//#include "confirmbox.h"
#include "create_account.h"
#include "Agree.h"
#include "Panel_PublicChat.h"
#include "Panel_Transfert.h"
#include "Panel_PrivChat.h"
#include "User.h"
#include "Download.h"
#include "ExplorerServer.h"
#include "ExplorerClient.h"
#include "NewsGroup.h"
#include "Rep.h"
#include "FileList.h"
#include "ChatText.h"
#include "Broadcast.h"
#include <wx/toolbar.h>
#include "FileInfo.h"
#include "PopupCreate.h"


//Private Define
