#include "Transaction.hpp"
#include <cstring>
#include <cstdio>

CTransactions::CTransactions(int Id)
{
  //cout << "Constructeur CTransactions" << endl;
  //cout << "id Transac: " << Id << endl;
  IdFlag = Id;
  buff = NULL;
  buffsize =  0;
}

CTransactions::CTransactions(char *val, int valsize)
{
  //cout << "Constructeur CTransactions(char*, int)\n";

  buff = NULL;
  buffsize =  0;
  parse_transac(val, valsize);
}

CTransactions::~CTransactions()
{
  //cout << "Destructeur CTransactions" << endl;
  if (buff != NULL)
    delete buff;
}

void CTransactions::parse_transac(char *val, int valsize)
{
  memcpy(&structure.grbg[0], val, 2);
  val += 2;
  memcpy(&structure.grbg[1], val, 2);
  structure.grbg[1] = ntohs(structure.grbg[1]);
  val += 2;
  memcpy(&structure.id, val, 4);
  structure.id = ntohl(structure.id);
  val += 4;
  memcpy(&structure.err, val, 4);
  structure.err = ntohl(structure.err);
  val += 4;
  memcpy(&structure.total_len, val, 4); 
  structure.total_len = ntohl(structure.total_len);
  val += 4;
  memcpy(&structure.data_len, val, 4);
  structure.data_len = ntohl(structure.data_len);
  val += 4;
  memcpy(&structure.nbr_param, val, 2);
  structure.nbr_param = ntohs(structure.nbr_param);
  val += 2;
  for (int i = 0; i < structure.nbr_param; i++) 
    {
      u_int16_t id; 
      u_int16_t size; 
      char	*data;

      memcpy(&id, val, 2);
      id = ntohs(id);
      val += 2;
      memcpy(&size, val, 2);
      size = ntohs(size);
      val += 2;
      data = new char[size];
      memcpy(data, val, size);
      val += size;
      structure.field.push_back(CField(id, size, 0, data));
      delete [] data;
    }
} 

void CTransactions::show_transac()
{
  printf("Flags: %d\n", ((u_int8_t *)(structure.grbg))[HDR_FLAG]);
  printf("Is reply: %d\n", ((u_int8_t *)(structure.grbg))[HDR_RPLY]);
  printf("Type: %d\n", structure.grbg[1]);
  printf("ID: %d\n", structure.id);
  printf("Error code: %d\n", structure.err);
  printf("Total size: %d\n", structure.total_len);
  printf("Data size: %d\n", structure.data_len);
  printf("Nbr of parameters: %d\n", structure.nbr_param); 
  
  char *tmps;
  int *tmpi;
  
  for (list<CField>::iterator it = structure.field.begin(); 
       it != structure.field.end(); 
       it++)
    {
      int i;
      char *data = it->getdata();
	  
      printf("Field ID: %d\n", it->id);
      printf("Field size: %d\n", it->size);
      //printf("Field type: %d\n", it->type);
      switch (it->type)
	{
	case STRG:
	  tmps = data;
	  printf("Field data (string): ");
	  for (i = 0; i < it->size; i++)
	    printf("%c", (unsigned char)tmps[i]);
	  printf("\n");
	  break;
	case LINT:
	  tmpi = (int *)data;
	  printf("Field data (usigned long int): ");
	  printf("%d\n", ntohl(*tmpi));
	  break;
	case SINT:
	  tmpi = (int *)data;
	  printf("Field data (usigned short int): ");
	  printf("%d\n", ntohs(*tmpi));
	  break;
	case BIN:
	  printf("Field data (binary): ");
	  goto def;
	case 0:
	  printf("Field data: ");
	default:
	def:
	  tmps = (char *)data;
	  for (i = 0; i < it->size; i++)
	    printf("%#x ", tmps[i]);
	  printf("\n");
	}
      it->deldata();
    }
}

void CTransactions::build_hdr(u_int16_t hdrid, u_int8_t is_reply)
{
  ((u_int8_t *)(structure.grbg))[HDR_FLAG] = HDR_FLAG;
  ((u_int8_t *)(structure.grbg))[HDR_RPLY] = is_reply;
  structure.grbg[1] = hdrid;
  structure.id = IdFlag;
  structure.err = 0;
  structure.total_len = 2;
  structure.data_len = 2;
  structure.nbr_param = 0;

}

void CTransactions::build_field(u_int16_t id, char *data, u_int16_t datasize, int type)
{
  structure.data_len += datasize + 4;

  structure.total_len = structure.data_len;
  structure.nbr_param++;
  
  structure.field.push_back(CField(id, datasize, type, data));
}

void CTransactions::build_field_b(u_int16_t id, vector<char> data, u_int16_t datasize)
{
  int i=0;
  char *buff = new char[data.size()];
  for (vector<char>::iterator it = data.begin(); it != data.end(); it++)
    buff[i++] = *it;
  build_field(id, buff, datasize, BIN);
  delete [] buff;
} 

void CTransactions::build_field_b(u_int16_t id, char *data, u_int16_t datasize)
{
  build_field(id, data, datasize, BIN);  
}

void CTransactions::build_field_i(u_int16_t id, int data) 
{
  if (data >= MAXUINT16)
    {
      u_int32_t convert;

      convert = htonl(data);
      build_field(id, (char *)&convert, (u_int16_t)sizeof(u_int32_t), LINT);
    }
  else
    {
      u_int16_t convert;
      
      convert = htons(data);
      build_field(id, (char *)&convert, sizeof(u_int16_t), SINT);
    }
}

void CTransactions::build_field_s(u_int16_t id, char *data)
{
  build_field(id, data, strlen(data), STRG);
}

#define COPY_SHORT(x)  {tmp_short = htons(x); memcpy(tmp, &tmp_short, sizeof(u_int16_t)); tmp += sizeof(u_int16_t);}
#define COPY_LONG(x)   {tmp_long = htonl(x); memcpy(tmp, &tmp_long, sizeof(u_int32_t)); tmp += sizeof(u_int32_t);}

void CTransactions::build_buff()
{
  char *tmp;
  u_int16_t tmp_short;
  u_int32_t tmp_long;
  
  buffsize = structure.data_len + HLHDR_SIZE;
  
  buff = new char[buffsize];
  tmp = buff;
  
  memcpy(tmp, &structure.grbg[0], sizeof(u_int16_t));
  tmp += sizeof(u_int16_t);
  
  COPY_SHORT(structure.grbg[1]);
  COPY_LONG(structure.id);
  COPY_LONG(structure.err);
  COPY_LONG(structure.total_len);
  COPY_LONG(structure.data_len);
  COPY_SHORT(structure.nbr_param);
  
  for (list<CField>::iterator it = structure.field.begin(); 
       it != structure.field.end(); 
       it++)
    {
      COPY_SHORT(it->id);
      COPY_SHORT(it->size);
      char *data = it->getdata();
      memcpy(tmp, data, it->size);
      tmp += it->size;
      it->deldata();
    }
}
