#include "FlFileObj.hpp"

CFlFileObj::CFlFileObj()
{
  FFO_buff = NULL;
  FFO_buff_size = 0;
}

CFlFileObj::CFlFileObj(char *buff, int size)
{
  FFO_buff = new char[size];
  memcpy(FFO_buff, buff, size);
  FFO_buff_size = size;
}

CFlFileObj::~CFlFileObj()
{
  delete [] FFO_buff;
}

void CFlFileObj::parse_FFO()
{
  if (FFO_buff)
    {
      parse_FFO_part1();
      parse_FFO_part2();
      parse_FFO_part3();
      parse_FFO_part4();
    }
  else
    cerr << "error: Flattened file object not initialised" << endl;  
}

char *CFlFileObj::make_FFO(string file)
{
  //cout << "Make Flattened File Object" << endl;
 
  string	rsvd;
  string	filename, ext, type("BINA"), crea("dosa");
  char		*ret;
  struct stat	sb;
  int		starti = file.find_last_of("/"), pt, len;

  filename = file.substr(starti + 1);
  pt = filename.find_last_of(".");
  if (pt != -1)
    {
      len = filename.length();
      len = len - pt;
      ext = filename.substr(pt, len);
      //cout << "extension: " << ext << endl;

      map<string,string>::iterator it, it2;
      if ((it = ExtToType.find(ext)) != ExtToType.end()
	  && (it2 = TypeToCrea.find(it->second)) != TypeToCrea.end())
	{
	  //cout << " ext: " << it->first << " - type: " << it->second << " - crea : " << it2->second << endl;
	  type = it->second;
	  crea = it2->second;
	}
      //else
	//cout << "not found" << endl;
    }
  if (stat(file.c_str(), &sb) == -1)
    {
      perror("stat() error");
      return NULL;
    }
  rsvd.assign(16, '\0');
  part1 = new CFFHeader((char*)"FILP", 1, (char *)rsvd.data(), 2);

  rsvd.assign(32, '\0');
  part3 = new CFFInfoFork(HL_OS, (char *)type.c_str(), (char *)crea.c_str(), (char *)rsvd.data(),  (char *)filename.c_str(), filename.length());

  rsvd.assign(4, '\0');
  part2 = new CFFForkHeader((char*)"INFO", 0, (char *)rsvd.data(), part3->name_size + 74);

  part4 = new CFFForkHeader((char*)"DATA", 0, (char *)rsvd.data(), sb.st_size);

  build_FFO_buff();

  ret = FFO_buff;

  return (ret);
}

void CFlFileObj::show_FFO()
{
  if (FFO_buff)
    {
      cout << "*****Flattened File Object*****" << endl;
      show_FFO_part1();
      show_FFO_part2();
      show_FFO_part3();
      show_FFO_part4();
      cout << "*******************************" << endl;
    }
  else
    cerr << "error: Flattened file object not initialised" << endl;  

}

int CFlFileObj::get_FFO_size()
{
  if (!FFO_buff)
    {
      cerr << "error: Flattened file object not initialised" << endl;
      return 0;
    } 
  else
    return (FFO_SIZE + part3->name_size);
}

void CFlFileObj::InitFiletype(map<string,string> map1, map<string,string> map2)
{
  ExtToType = map1;
  TypeToCrea = map2;
}

void CFlFileObj::parse_FFO_part1()
{
  char *tmp;
  u_int16_t count, vers;
    
  tmp = FFO_buff;
  memcpy(&vers, tmp + 4, 2);
  vers = ntohs(vers);
  memcpy(&count, tmp + 22, 2);
  count = ntohs(count);
  part1 = new CFFHeader(tmp, vers, tmp + 6, count);
}

void CFlFileObj::parse_FFO_part2()
{
  char *tmp;
  u_int32_t ctype, dsize;
  
  tmp = FFO_buff + 24;
  memcpy(&ctype, tmp + 4, 4);
  ctype = ntohl(ctype);
  memcpy(&dsize, tmp + 12, 4);
  dsize = ntohl(dsize);
  part2 = new CFFForkHeader(tmp, ctype, tmp + 8, dsize);

}

void CFlFileObj::parse_FFO_part3()
{
  char *tmp;
  u_int16_t namesize;
  
  tmp = FFO_buff + 110;
  memcpy(&namesize, tmp, 2);
  namesize = ntohs(namesize);
  tmp = FFO_buff + 40;
  part3 = new CFFInfoFork(tmp, tmp + 4, tmp + 8, tmp + 20, tmp + 72, namesize);
  memcpy(&part3->flags, tmp + 12, 4);
  memcpy(&part3->platform_flags, tmp + 16, 4);
  part3->rsvd = part3->setdata(tmp + 20, 32);
  part3->create_date = part3->setdata(tmp + 52, 8);
  part3->modify_date = part3->setdata(tmp + 60, 8);
  memcpy(&part3->name_script, tmp + 68, 2);
}

void CFlFileObj::parse_FFO_part4()
{
  char *tmp;
  u_int16_t namesize;
  
  tmp = FFO_buff + 110;
  memcpy(&namesize, tmp, 2);
  namesize = ntohs(namesize);
  //cout << " parse_FFO_part3 - Name size: " << namesize << endl;
  tmp = FFO_buff + 114 + namesize;

  u_int32_t ctype, dsize;

  memcpy(&ctype, tmp + 4, 4);
  ctype = ntohl(ctype);
  memcpy(&dsize, tmp + 12, 4);
  dsize = ntohl(dsize);
  part4 = new CFFForkHeader(tmp, ctype, tmp + 8, dsize);
}

void CFlFileObj::show_FFO_part1()
{
  cout << "- Flat file header" << endl ;
  part1->show();
}

void CFlFileObj::show_FFO_part2()
{
  cout << "- Flat file information fork header" << endl ;
  part2->show();
}

void CFlFileObj::show_FFO_part3()
{
  cout << "- Flat file information fork" << endl ;
  part3->show();
}

void CFlFileObj::show_FFO_part4()
{
  cout << "- Flat file data fork header" << endl ;
  part4->show();
}

#define COPY_SHORT(x)	{tmp_short = htons(x); memcpy(tmp, &tmp_short, sizeof(u_int16_t)); tmp += sizeof(u_int16_t);}
#define COPY_LONG(x)	{tmp_long = htonl(x); memcpy(tmp, &tmp_long, sizeof(u_int32_t)); tmp += sizeof(u_int32_t);}
#define COPY_OTHER(x, y)	{memcpy(tmp, x, y); tmp += y;}

void CFlFileObj::build_FFO_buff()
{
  char *tmp;
  u_int16_t tmp_short;
  u_int32_t tmp_long;
  
  FFO_buff_size = part3->name_size + FFO_SIZE;
  FFO_buff = new char[FFO_buff_size];
  tmp = FFO_buff;
  
  COPY_OTHER(part1->getdata(part1->format), 4);
  COPY_SHORT(part1->version);
  COPY_OTHER(part1->getdata(part1->rsvd), 16);
  COPY_SHORT(part1->fork_count);

  COPY_OTHER(part2->getdata(part2->fork_type), 4);
  COPY_LONG(part2->compression_type);
  COPY_OTHER(part2->getdata(part2->rsvd), 4);
  COPY_LONG(part2->data_size);
  
  COPY_OTHER(part3->getdata(part3->platform), 4);
  COPY_OTHER(part3->getdata(part3->type_signature), 4);
  COPY_OTHER(part3->getdata(part3->creator_signature), 4);
  COPY_LONG(part3->flags);
  COPY_LONG(part3->platform_flags);
  COPY_OTHER(part3->getdata(part3->rsvd), 32);
  COPY_OTHER(part3->getdata(part3->create_date), 8);
  COPY_OTHER(part3->getdata(part3->modify_date), 8);
  COPY_SHORT(part3->name_script);
  COPY_SHORT(part3->name_size);
  COPY_OTHER(part3->name.c_str(), part3->name_size);
  COPY_SHORT(part3->ukn);

  COPY_OTHER(part4->getdata(part4->fork_type), 4);
  COPY_LONG(part4->compression_type);
  COPY_OTHER(part4->getdata(part4->rsvd), 4); 
  COPY_LONG(part4->data_size);
}
