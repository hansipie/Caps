#include "CUser.hpp"

CUser::CUser(int _fd)
{
  connected = false;
  fd = _fd;
  name = "";
  iconId = 0;
  userId = 0;
  version = 0;
  userFlags = 0;
  auto_rep = "";
  isaway = false;
  isadmin = false;
  rpm = false;
  rpc = false;
  ar = false;
  /*invariables*/
  confname= "";
  login = "";
  password = "";
  for (int i = 0; i < 41; i++)
    access[i] = false;
  /**/  
}

CUser::~CUser()
{
}

void CUser::Serialize(string file)
{
  //cout << "Serialize CUser in " << file << endl;

  ofstream accountfile (file.c_str(), ios::trunc);
  if (accountfile.is_open())
  {
    accountfile << "name = \"" << confname << "\"" << endl;
    accountfile << "login = \"" << login << "\"" << endl;
    accountfile << "password = \"" << password << "\"" << endl;
    
    string bits;
    bits.assign(64, '0');
    for (int i = 0; i < 41; i++)
      bits.replace(i, 1, (access[i]?"1":"0"));
    
    accountfile << "DeleteFile = " <<  bits.at(0) << endl;
    accountfile << "UploadFile = " <<  bits.at(1) << endl;
    accountfile << "DownloadFile = " <<  bits.at(2) << endl;
    accountfile << "RenameFile = " <<  bits.at(3) << endl;
    
    accountfile << "MoveFile = " <<  bits.at(4) << endl;
    accountfile << "CreateFolder = " <<  bits.at(5) << endl;
    accountfile << "DeleteFolder = " <<  bits.at(6) << endl;
    accountfile << "RenameFolder = " <<  bits.at(7) << endl;

    accountfile << "MoveFolder = " <<  bits.at(8) << endl;
    accountfile << "ReadChat = " <<  bits.at(9) << endl;
    accountfile << "SendChat = " <<  bits.at(10) << endl;
    accountfile << "OpenChat = " <<  bits.at(11) << endl;

    accountfile << "CloseChat = " <<  bits.at(12) << endl;
    accountfile << "ShowInList = " <<  bits.at(13) << endl;
    accountfile << "CreateUser = " <<  bits.at(14) << endl;
    accountfile << "DeleteUser = " <<  bits.at(15) << endl;

    accountfile << "OpenUser = " <<  bits.at(16) << endl;
    accountfile << "ModifyUser = " <<  bits.at(17) << endl;
    accountfile << "ChangeOwnPass = " <<  bits.at(18) << endl;
    accountfile << "SendPrivMsg = " <<  bits.at(19) << endl;

    accountfile << "NewsReadArt = " <<  bits.at(20) << endl;
    accountfile << "NewsPostArt = " <<  bits.at(21) << endl;
    accountfile << "DisconUser = " <<  bits.at(22) << endl;
    accountfile << "CannotBeDiscon = " <<  bits.at(23) << endl;

    accountfile << "GetClientInfo = " <<  bits.at(24) << endl;
    accountfile << "UploadAnywhere = " <<  bits.at(25) << endl;
    accountfile << "AnyName = " <<  bits.at(26) << endl;
    accountfile << "NoAgreement = " <<  bits.at(27) << endl;

    accountfile << "SetFileComment = " <<  bits.at(28) << endl;
    accountfile << "SetFolderComment = " <<  bits.at(29) << endl;
    accountfile << "ViewDropBoxes = " <<  bits.at(30) << endl;
    accountfile << "MakeAlias = " <<  bits.at(31) << endl;

    accountfile << "Broadcast = " <<  bits.at(32) << endl;
    accountfile << "NewsDeleteArt = " <<  bits.at(33) << endl;
    accountfile << "NewsCreateCat = " <<  bits.at(34) << endl;
    accountfile << "NewsDeleteCat = " <<  bits.at(35) << endl;

    accountfile << "NewsCreateFldr = " <<  bits.at(36) << endl;
    accountfile << "NewsDeleteFldr = " <<  bits.at(37) << endl;
    accountfile << "UploadFldr = " <<  bits.at(38) << endl;
    accountfile << "DownloadFldr = " <<  bits.at(39) << endl;

    accountfile << "CanSendMsg = " <<  bits.at(40) << endl;
    accountfile.close();
  }
}
