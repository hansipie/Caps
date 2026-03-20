#include "Serv.hpp"

void CServ::ParseConfLine(string line)
{
  int index = line.find_first_of('=');
  if (index == -1)
    return;
  string variable = line.substr(0, index);
  string value = line.substr(index, line.size());
  //   cout << "variable : [" << variable << "]" << endl;
  //   cout << "value : [" << value << "]" << endl;
  if ((int)variable.find("ServerName", 0, 10) >= 0)
    {
      //      cout << "get_servername" << endl;
      ServConf.ServerName = get_servername(value);
      return;
    }
  if ((int)variable.find("Port", 0, 4) >= 0)
    {
      //      cout << "get_serverport" << endl;
      ServConf.Port = get_serverport(value);
      return;
    }
  if ((int)variable.find("Agreement", 0, 9) >= 0)
    {
      //      cout << "get_agreementpath" << endl;
      ServConf.Agreement = get_path(value, 0); 
    }
  if ((int)variable.find("FilesFolder", 0, 12) >= 0)
    {
      //      cout << "get_FilesFolder" << endl;
      ServConf.FilesFolder = get_path(value, 1); 
    }
  if ((int)variable.find("FilesType", 0, 9) >= 0)
    {
      //      cout << "get_FilesType" << endl;
      ServConf.FilesType = get_path(value, 2); 
    }
  if ((int)variable.find("Videolan", 0, 8) >= 0)
    {
      //      cout << "get_Videolan" << endl;
      ServConf.Videolan = get_path(value, -1); 
    }

}

string CServ::get_servername(string value)
{
  int starti = value.find_first_of("\"") ;
  int endi = value.find_last_of("\"");
  if ((starti == -1) || (endi == -1) || (starti == endi))
    {
      cerr << "Error in configuration file: invalid ServerName value" << endl;
      if (DEBUG_L1)
	cout << "ServerName set to default value: " << ServConf.ServerName << endl;
      return ServConf.ServerName;
    }
  return value.substr(starti + 1, endi - starti - 1);
}

int CServ::get_serverport(string value)
{
  int starti = value.find_first_of("0123456789") ;
  int endi = value.find_last_of("0123456789");
  if ((starti == -1) || (endi == -1) || (starti == endi))
    {
      cerr << "Error in configuration file: invalid Port value" << endl;
      if (DEBUG_L1)
	cout << "Port set to default value: " << ServConf.Port << endl;
      return ServConf.Port;
    }
  string port = value.substr(starti, endi - starti +1);
  int intrus = port.find_first_not_of("0123456789");
  if (intrus != -1)
    {
      cerr << "Error in configuration file: invalid Port value" << endl;
      if (DEBUG_L1)
	cerr << "Port set to default value: " << ServConf.Port << endl;
      return ServConf.Port;
    }  
  //  cout << "Port: " << port << endl;
  return atoi(port.c_str());
}

string CServ::get_path(string value, int flag)
{
  int starti = value.find_first_of("\"") ;
  int endi = value.find_last_of("\"");
  if ((starti == -1) || (endi == -1) || (starti == endi))
    {
      switch (flag)
	{
	case 2:
	  cerr << "Error in configuration file: invalid FilesType value" << endl;
	  if (DEBUG_L1)
	    cout << "FilesType path set to default value: " << ServConf.FilesType << endl;
	  return ServConf.FilesType;
	case 1:
	  cerr << "Error in configuration file: invalid FilesFolder value" << endl;
	  if (DEBUG_L1)
	    cout << "FilesFolder path set to default value: " << ServConf.FilesFolder << endl;
	  return ServConf.FilesFolder;
	case 0:
	  cerr << "Error in configuration file: invalid Agreement value" << endl;
	  if (DEBUG_L1)
	    cout << "Agreement path set to default value: " << ServConf.Agreement << endl;
	  return ServConf.Agreement;
	default:
	  cerr << "Error in configuration file" << endl;
	  if (DEBUG_L1)
	    cout << "set to default value: \"\""<< endl;
	  return "";
	}
    }
  return value.substr(starti + 1, endi - starti - 1);
}

void CServ::GetFilesType()
{
  ifstream conf_file (ServConf.FilesType.c_str());
  if (!conf_file.is_open())
    { 
      cout << "Error opening FilesType file:" << endl; 
      exit(1); 
    }
  char buffer[256];
  string line, elem1, elem2, elem3;
  int starti, endi, pt, len, i;
  while (!conf_file.eof())
    {
      conf_file.getline(buffer, 256, '\n');
      line = buffer;
      pt = line.find_first_of(".");
      starti = line.find_first_of(":");
      endi = line.find_last_of(":");
      if ((pt == -1) || (starti == -1) || (endi == -1) || (starti == endi))
	  cerr << "Error in FileType file: invalid line" << endl;
      else
	{
	  elem1 = line.substr(pt, starti - pt);
	  elem2 = line.substr(starti + 1, endi - starti - 1);
	  elem3 = line.substr(endi + 1, 4);
	  if ((len = elem2.length()) < 4)
	    {
	      len = 4 - len;
	      for (i = 0; i < len; i++)
		elem2.append(" ");
	    }
	  if ((len = elem3.length()) < 4)
	    {
	      len = 4 - len;
	      for (i = 0; i < len; i++)
		elem3.append(" ");
	    }
	  if (DEBUG_L1)
	    cout << "[" << elem1 << "][" << elem2 << "][" << elem3 << "]" << endl;
	  ExtToType[elem1] = elem2;
	  TypeToCrea[elem2] = elem3;
	}
    }
  conf_file.close();
  
}

int CServ::GetAccounts(string path)
{
  DIR		*dir;
  string	file;
  struct dirent *entry;
  struct stat	sb;
  
  if ((dir = opendir(path.c_str())) == NULL)
    {
      perror("opendir() error");
      return 0;
    }
  else 
    {
      while ((entry = readdir(dir)) != NULL)
	{
	  file = path + entry->d_name;
	  if (stat(file.c_str(), &sb) == -1)
	    perror("stat() error");
	  else
	    if (sb.st_mode & S_IFREG)
	      ParseAccount(file);
	}
      closedir(dir);
    }
  return 1;
}

void CServ::ParseAccount(string account)
{
  char buffer[256];
  string line;
  ifstream account_file (account.c_str());

  if (!account_file.is_open())
    { 
      cerr << "Error opening account file: " << account.c_str() << endl; 
      return; 
    }
  if (DEBUG_L1)
    cout << "opening account file: " << account.c_str() << endl; 
  if (AccountsLst.empty() || AccountsLst.back().login != "")
    AccountsLst.push_back(CUser(-1));
  while (!account_file.eof())
    {
      account_file.getline(buffer, 256, '\n');
      line = buffer;
      ParseAccountLine(line);
    }
  account_file.close();
}

void CServ::ParseAccountLine(string line)
{
  CUser& NewUser = AccountsLst.back();
  int index = line.find_first_of('=');
  if (index == -1)
    return;
  string variable = line.substr(0, index);
  string value = line.substr(index, line.size());
  
  /*
  ** doivent etre ici pour eviter un conflit avec "name" 
  */
  if ((int)variable.find("RenameFile", 0, 10) >= 0)
    {
      NewUser.access[3] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("RenameFolder", 0, 12) >= 0)
    {
      NewUser.access[7] = get_intval(variable, value);
      return;
    }
  /*
  ** end
  */
 
  if ((int)variable.find("name", 0, 4) >= 0)
    {
      NewUser.name = get_stringval(variable, value);
      NewUser.confname = NewUser.name;
      return;
    }
  if ((int)variable.find("login", 0, 5) >= 0)
    {
      NewUser.login = get_stringval(variable, value); 
      return;
    }
  if ((int)variable.find("password", 0, 8) >= 0)
    {
      NewUser.password = get_stringval(variable, value); 
      return;
    }
  if ((int)variable.find("DeleteFile", 0, 10) >= 0)
    {
      NewUser.access[0] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("UploadFile", 0, 10) >= 0)
    {
      NewUser.access[1] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("DownloadFile", 0, 12) >= 0)
    {
      NewUser.access[2] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("MoveFile", 0, 8) >= 0)
    {
      NewUser.access[4] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("CreateFolder", 0, 12) >= 0)
    {
      NewUser.access[5] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("DeleteFolder", 0, 12) >= 0)
    {
      NewUser.access[6] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("MoveFolder", 0, 10) >= 0)
    {
      NewUser.access[8] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("ReadChat", 0, 8) >= 0)
    {
      NewUser.access[9] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("SendChat", 0, 8) >= 0)
    {
      NewUser.access[10] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("OpenChat", 0, 8) >= 0)
    {
      NewUser.access[11] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("CloseChat", 0, 9) >= 0)
    {
      NewUser.access[12] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("ShowInList", 0, 10) >= 0)
    {
      NewUser.access[13] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("CreateUser", 0, 10) >= 0)
    {
      NewUser.access[14] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("DeleteUser", 0, 10) >= 0)
    {
      NewUser.access[15] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("OpenUser", 0, 8) >= 0)
    {
      NewUser.access[16] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("ModifyUser", 0, 10) >= 0)
    {
      NewUser.access[17] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("ChangeOwnPass", 0, 13) >= 0)
    {
      NewUser.access[18] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("SendPrivMsg", 0, 11) >= 0)
    {
      NewUser.access[19] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("NewsReadArt", 0, 11) >= 0)
    {
      NewUser.access[20] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("NewsPostArt", 0, 11) >= 0)
    {
      NewUser.access[21] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("DisconUser", 0, 10) >= 0)
    {
      NewUser.access[22] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("CannotBeDiscon", 0, 14) >= 0)
    {
      NewUser.access[23] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("GetClientInfo", 0, 13) >= 0)
    {
      NewUser.access[24] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("UploadAnywhere", 0, 14) >= 0)
    {
      NewUser.access[25] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("AnyName", 0, 7) >= 0)
    {
      NewUser.access[26] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("NoAgreement", 0, 11) >= 0)
    {
      NewUser.access[27] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("SetFileComment", 0, 14) >= 0)
    {
      NewUser.access[28] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("SetFolderComment", 0, 16) >= 0)
    {
      NewUser.access[29] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("ViewDropBoxes", 0, 13) >= 0)
    {
      NewUser.access[30] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("MakeAlias", 0, 9) >= 0)
    {
      NewUser.access[31] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("Broadcast", 0, 9) >= 0)
    {
      NewUser.access[32] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("NewsDeleteArt", 0, 13) >= 0)
    {
      NewUser.access[33] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("NewsCreateCat", 0, 13) >= 0)
    {
      NewUser.access[34] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("NewsDeleteCat", 0, 13) >= 0)
    {
      NewUser.access[35] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("NewsCreateFldr", 0, 14) >= 0)
    {
      NewUser.access[36] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("NewsDeleteFldr", 0, 14) >= 0)
    {
      NewUser.access[37] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("UploadFldr", 0, 10) >= 0)
    {
      NewUser.access[38] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("DownloadFldr", 0, 12) >= 0)
    {
      NewUser.access[39] = get_intval(variable, value);
      return;
    }
  if ((int)variable.find("CanSendMsg", 0, 10) >= 0)
    {
      NewUser.access[40] = get_intval(variable, value);
      return;
    }
}

string CServ::get_stringval(string variable, string value)
{
  int starti = value.find_first_of("\"") ;
  int endi = value.find_last_of("\"");
  if ((starti == -1) || (endi == -1))
    {
      cerr << "Error in account file: invalid " << variable <<" value" << endl;
      if (DEBUG_L1)
	cout << "set to NULL value. " <<  endl;
      return "";
    }
  return value.substr(starti + 1, endi - starti - 1);
}

int CServ::get_intval(string variable, string value)
{
  //      cout << "variable : [" << variable << "] " << "value : [" << value << "]" << endl;

  int starti = value.find_first_of("0123456789") ;
  int endi = value.find_last_of("0123456789");
  if ((starti == -1) || (endi == -1))
    {
      cerr << "Error in account file: invalid " << variable <<" value" << endl;
      if (DEBUG_L1)
	cout << "set to NULL value. " <<  endl;
      return 0;
    }
  string port = value.substr(starti, endi - starti +1);
  int intrus = port.find_first_not_of("0123456789");
  if (intrus != -1)
    {
      cerr << "Error in account file: invalid " << variable <<" value" << endl;
      if (DEBUG_L1)
	cout << "set to NULL value. " <<  endl;
      return 0;
    }
  return atoi(port.c_str());
}

void CServ::GetLower(string& str)
{
  for (int i=0; i < (int)str.length(); i++)
    str[i] = tolower(str[i]);
}

unsigned char	CServ::btoc(string bits)
{
  int mult = 0;
  int res = 0;
  int k = 0;
  for (int i = (int)bits.length()-1; i >= 0; i--)
    {
      mult = (int)pow(2, (double)i);
      if (bits.at(k) == '1')
	res+=mult;
      k++;
    }
  return ((unsigned char)res);
}


vector<char> CServ::BitsField(string field)
{
  if (DEBUG_L1)
    cout << "#############CServ BitsField###############" << endl;
  vector<char> result;
  if (DEBUG_L1)
    cout << "field.length() : " << field.length() << endl;
  if ((field.length()%8) != 0)
    {
      cerr << "error: BitsField with bad size" << endl;
      return (result);
    }
  int iter = field.length()/8;
  for (int i = 0; i < iter; i++)
    {
      string bits = field.substr(i*8, 8);
      result.push_back(btoc(bits));
      if (DEBUG_L1)
	cout << "octet[" << i << "]=" << (int)btoc(bits) << endl; 
    }
  if (DEBUG_L1)
    cout << "##########################################" << endl;
  return (result);
}

string CServ::get_HLCode(char *src, int len)
{
  char      *tmp;
  string    res;
  int       i;

  tmp = new char[len + 1];
  memcpy(tmp, src,len);
  if (DEBUG_L1)
    cout << "hlcode: " ;
  for (i = 0; i < len; i++)
    {
      tmp[i]=~tmp[i];
      if (DEBUG_L1)
	cout << tmp[i];
    }
  if (DEBUG_L1)
    cout << endl;
  tmp[i] = 0;
  res.assign(tmp);
  delete [] tmp;
  return res;
}

