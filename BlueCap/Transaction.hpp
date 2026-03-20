#ifndef __transaction_hpp
#define __transaction_hpp

using namespace std;

#include <vector>
#include <list>
#include <iostream>

#ifdef __WIN32__
    #include <ws2tcpip.h>
#else
    #include <netinet/in.h>
#endif

#include "Hotline.hpp"
#include "Systypes.hpp"

class           CField
{
 public:
  CField(int _id, int _size, int _type, char *_data)
    {
      //cout << "Constructeur CField\n";
      id = _id;
      size = _size;
      type = _type;
      for(int i=0; i<_size; i++)
	vdata.push_back(_data[i]);
    }
  char *getdata()
  {
    int i=0;
    data = new char[size];
    //cout << "CBuffer::getdata() - malloc" << endl;
    for (vector<char>::iterator it = vdata.begin(); it != vdata.end(); it++)
      data[i++] = *it;
    return data;
  }
  void deldata()
  {
    //cout << "CBuffer::deldata() - free" << endl;
    delete [] data;
  }

  u_int16_t	id;
  u_int16_t	size;
  int		type;
  char		*data;
  vector<char>	vdata;
};

class           CTransac
{
public:
  CTransac()
  {      
    //cout << "Constructeur CTransac\n";
  }
  u_int16_t     grbg[2];
  u_int32_t     id;
  u_int32_t     err;
  u_int32_t     total_len;
  u_int32_t     data_len;
  u_int16_t     nbr_param;
  list<CField>  field;
};

class           CTransacLst
{
public:
  CTransacLst(CTransac _structure)
  {
    structure = _structure;
  }
  CTransac structure;
};

class CTransactions
{
 private:
  void		parse_transac(char *, int);
  
 public:
  CTransac		structure;
  char			*buff;
  int			buffsize;
  int			IdFlag;
  
  CTransactions(int);
  CTransactions(char *, int);
  ~CTransactions();

  void	build_field_i(u_int16_t, int);
  void	build_field_s(u_int16_t, char *);
  void	build_field_b(u_int16_t, vector<char>, u_int16_t);
  void	build_field_b(u_int16_t, char *, u_int16_t);
  void	build_field(u_int16_t, char *, u_int16_t, int);
  void	build_hdr(u_int16_t, u_int8_t);
  void  build_buff();
  void	show_transac();
};

#endif
