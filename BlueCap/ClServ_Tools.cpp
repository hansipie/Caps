#include "Serv.hpp"

string	CClServ::ctob(unsigned char ti)
{
  int		q, r, cpt;
  string	bits;
  
  cpt = 7;
  bits.assign(8, '0');
  do
    {
      q = ti / 2;
      r = ti % 2;
      ti = q;
      if (r != 0)
	bits.replace(cpt, 1, "1");
      cpt--;
    }
  while(ti>0);
  return (bits);
}

unsigned char	CClServ::btoc(string bits)
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

void CClServ::ChangeUserFlags(int flag, string val)
{
  // on suppose que le user flag en code sur 1 octet
  // alors que en realite il est code sur 2 octets, 
  // car le premier octet est a 0
  string tmp(ctob(User->userFlags));
  if (DEBUG_L1)
    cout << "user flags before change  :" << tmp << endl;
  tmp.replace(flag, 1, val);
  if (DEBUG_L1)
    cout << "user flags after change  :" << tmp << endl;
  User->userFlags = (u_int16_t)btoc(tmp);
}

void CClServ::UpdateUserFlags(bool update)
{
  if (User->access[22])
    {
      if (DEBUG_L1)
	cout << " setting 'is admin' flag for " << User->name << endl;
      ChangeUserFlags(6, "1");
    }

  for (int k = 0; k < 41; k++)
    {
      if (k == 27)
	continue;
      if (User->access[k] == false)
	break;
      if (k == 40)
	{
	  if (DEBUG_L1)
	    cout << " setting all rights flag for " << User->name << endl;
	  ChangeUserFlags(3, "1");
	}
    }

  for (int l = 0; l < 41; l++)
    {
      if (User->access[l] == true)
	break;
      if (l == 40)
	{
	  if (DEBUG_L1)
	    cout << " setting no rights flags for " << User->name << endl;
	  ChangeUserFlags(2, "1");
	}
    }
  if (update)
    TabPtrFunc.push_back(&CClServ::MakeUserChange);
}

void CClServ::SetFlags113(string flags)
{
  if (flags.at(7) == '1')
    {
      User->rpm=true;
      ChangeUserFlags(5, "1");
    }
  else
    {
      User->rpm=false;
      ChangeUserFlags(5, "0");
    }

  if (flags.at(6) == '1')
    {
      User->rpc=true;
      ChangeUserFlags(4, "1");
    }
  else
    {
      User->rpc=false;
      ChangeUserFlags(4, "0");
    }
  if (flags.at(5) == '1')
    {
      User->ar=true;
    }
  else
    User->ar=false;
  if (DEBUG_L1)
    cout << " user flag: " <<  User->userFlags << endl;
}

vector<char> CClServ::BitsField(string field)
{
  if (DEBUG_L1)
    cout << "#############BitsField###############" << endl;
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
    cout << "####################################" << endl;
  return (result);
}

void CClServ::ls(string path)
{
  if (DEBUG_L1)
    cout << "Server file list - path: " << path << endl;

  DIR		*dirp;
  struct dirent *dirent;

  dirp = opendir(path.c_str());
  while ((dirent = readdir(dirp)))
    cout << dirent->d_name << endl;
}

char *CClServ::read_agreement(char *agreement)
{
  char buffer[256];
  string agreementtext;
  char myeol[3];
  ifstream agreementfile (agreement);


  myeol[0] = ' '; // should be myeol[0] = 10; 
  myeol[1] = 13;
  myeol[2] = 0;
  if (!agreementfile.is_open())
    { 
      cerr << "Error opening file: no agreement" << endl; 
      return(""); 
    }
  while (!agreementfile.eof())
    {
      agreementfile.getline(buffer, 256, '\n');
      agreementtext.append(buffer);
      agreementtext.append(myeol);
    }
  return ((char *)agreementtext.c_str());
}

int CClServ::FileSize(string File)
{
  struct stat	sb;
  
  if (stat(File.c_str(), &sb) == -1)
    {
      perror("stat() error");
      return -1;
    }
  else
    {
      if (DEBUG_L1)
	cout << " size of file " << File << " : " << sb.st_size << " bytes" << endl;  
      return sb.st_size;
    }
}

string CClServ::GetExtension(string file)
{
  string ext;

  int pt = file.find_last_of(".");
  if (pt != -1)
    {
      int len = file.length();
      len = len - pt;
      ext = file.substr(pt, len);
      if (DEBUG_L1)
	cout << " extension: " << ext << endl;
    }
  return(ext);
}

void CClServ::print_ascii(char *val, int len)
{
  cout << "datagram (text mode): ";
  for (int i=0; i<len; i++)
    if ((val[i] >= 32) && (val[i] <= 126))
      cout << "[" << val[i] << "]";
    else
      cout << "[\\" << (int)val[i] << "]";
  std::cout << "\n";
}

void CClServ::print_hexa(char *val, int len)
{
  printf("datagram (hexa mode): ");
  for (int i=0; i<len; i++)
    printf("0x%x ", val[i]);
  printf("\n");
}

string CClServ::get_HLCode(char *src, int len)
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

void CClServ::ClearTemps()
{
  c_tempstr.clear();
  c_error.clear();
  c_tempint16 = 0;
  c_tempint32 = 0;
  c_tempint = 0;
  c_tempbool = false;
}    

bool	CClServ::Is_movie(string ext)
{
  map<string,string> ExtToType, TypeToCrea;
  string type("NULL"), crea("NULL");

  ExtToType = Server->ExtToType;
  TypeToCrea = Server->TypeToCrea;
  map<string,string>::iterator it, it2;
  if ((it = ExtToType.find(ext)) != ExtToType.end()
      && (it2 = TypeToCrea.find(it->second)) != TypeToCrea.end())
    {
      cout << " Is movie:" << endl;
      cout << "  ext: " << it->first << " - type: " << it->second << " - crea : " << it2->second << endl;
      type = it->second;
      crea = it2->second;
    }
  if ((crea == "NULL") || (crea != "TVOD"))
    return false;
  else
    return true;
}
