#ifndef __flfileobj_hpp
#define __flfileobj_hpp

using namespace std;

#include <string.h>
#include <cstdio>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef __WIN32__
    #include <ws2tcpip.h>
#else
    #include <netinet/in.h>
#endif

#include "Systypes.hpp"

#define FFO_SIZE 130

#ifdef __ppc__
    #define HL_OS "AMAC"
#else
    //ifdef __i386__
    #define HL_OS "MWIN"
#endif

class CMyVectorTools
{
public:
  CMyVectorTools() 
  {
  }

  vector<char>	setdata(char *data, int size)
  {
    char		*tmp;
    vector<char>	ret;
    
    tmp = data;
    for (int i = 0; i < size; i++)
      {
	ret.push_back(*tmp);
	tmp++;
      }
    return ret;
  }

  char *getdata(vector<char> data)
  {
    char *ret = new char[data.size() + 1];
    int	i = 0;

    for (vector<char>::iterator it = data.begin(); 
	 it != data.end(); it++)
      ret[i++] = *it;
    ret[i] = 0;
    return ret;
  }  
  
  void show_vector(vector<char> data)
  {
    for (vector<char>::iterator it = data.begin(); 
	 it != data.end(); it++)
      //cout <<  (int)*it << " ";
      printf("0x%x ", *it);
  }
};

class CFFHeader: public CMyVectorTools
{
public:
  CFFHeader(char *_format, u_int16_t _vers, 
	    char *_rsvd, u_int16_t _count)
  {
    version = _vers;
    fork_count = _count;
    format = setdata(_format, 4);
    rsvd = setdata(_rsvd, 16);
  }
  void show()
  {
    cout << "format: " << getdata(format) << endl;
    cout << "version: " << version << endl;
    cout << "rsvd: "; show_vector(rsvd);  cout << endl;
    cout << "fork count: " << fork_count << endl;
  }
  vector<char>	    format;
  u_int16_t	    version;
  vector<char>	    rsvd;
  u_int16_t	    fork_count;
};

class CFFForkHeader: public CMyVectorTools
{
public:
  CFFForkHeader(char *_fork_type, u_int32_t _compression_type, 
		char *_rsvd, u_int32_t _data_size)
  {
    compression_type = _compression_type;
    data_size = _data_size;
    fork_type = setdata(_fork_type, 4);
    rsvd = setdata(_rsvd, 4);
  }
  void show()
  {
    cout << "fork type: " << getdata(fork_type) << endl;
    cout << "compression type: " << compression_type << endl;
    cout << "rsvd: "; show_vector(rsvd);  cout << endl;
    cout << "data size: " << data_size << endl;
  }
  vector<char>	    fork_type;
  u_int32_t	    compression_type;
  vector<char>	    rsvd;
  u_int32_t	    data_size;
};

class CFFInfoFork: public CMyVectorTools
{
public:
  CFFInfoFork(char *_platform, char *_type_signature,
	      char *_creator_signature, char *_rsvd,
	      char *_name, u_int16_t nsize)
  {
    string nulldate;
    nulldate.assign(8, '\0');
    platform = setdata(_platform, 4);
    type_signature = setdata(_type_signature, 4);
    creator_signature = setdata(_creator_signature, 4);
    rsvd = setdata(_rsvd, 32);
    name_size = nsize;
    name.assign(_name, name_size);
    flags = 0;
    create_date = setdata((char *)nulldate.c_str(), 8);
    modify_date = setdata((char *)nulldate.c_str(), 8);
    platform_flags = 0;
    name_script = 0;
    ukn = 0;
  }
  void show()
  {
    cout << "platform: " << getdata(platform) << endl;
    cout << "type signature: " << getdata(type_signature) << endl;
    cout << "creator signature: " << getdata(creator_signature) << endl;
    cout << "flags: " << flags << endl;
    cout << "platform flags: " << platform_flags << endl;
    cout << "rsvd: "; show_vector(rsvd);  cout << endl;
    cout << "create date: "; show_vector(create_date); cout << endl;
    cout << "modify date: "; show_vector(modify_date); cout << endl;
    cout << "name script: " << name_script << endl;
    cout << "name size: " << name_size << endl;
    cout << "name: " << name << endl;
    cout << "??: " << ukn << endl;
  }
  vector<char>	platform;
  vector<char>	type_signature;
  vector<char>	creator_signature;
  u_int32_t	flags;
  u_int32_t	platform_flags;
  vector<char>	rsvd;
  vector<char>	create_date;
  vector<char>	modify_date;
  u_int16_t	name_script;
  u_int16_t	name_size;
  string	name;
  u_int16_t	ukn;
};

class CFlFileObj
{
public:
  char		*FFO_buff;
  int		FFO_buff_size;

  map<string,string>	ExtToType;
  map<string,string>	TypeToCrea;

  CFlFileObj();
  CFlFileObj(char *, int);
  ~CFlFileObj();

  void	parse_FFO();
  char	*make_FFO(string);
  void	show_FFO();
  int	get_FFO_size();
  void	InitFiletype(map<string,string>, map<string,string>);
  u_int32_t	get_FileSize() {return part4->data_size;}

private:
  CFFHeader	*part1;
  CFFForkHeader *part2;
  CFFInfoFork	*part3;
  CFFForkHeader	*part4;

  void parse_FFO_part1();
  void parse_FFO_part2();
  void parse_FFO_part3();
  void parse_FFO_part4();
  void show_FFO_part1();
  void show_FFO_part2();
  void show_FFO_part3();
  void show_FFO_part4();
  void build_FFO_buff();
};

#endif
