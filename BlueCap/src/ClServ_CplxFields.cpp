#include "Serv.hpp"

vector<string> CClServ::Field202(char *data, int size)
{
  char			ukn, *name;
  string		dir;
  u_int16_t		dir_level, dir_name_size;
  vector<string>	dirs;

  // dir level
  if (DEBUG_L1)
    cout << " field 202 - size: " << size  <<endl;
  memcpy(&dir_level, data, sizeof(u_int16_t));
  dir_level = ntohs(dir_level);
  if (DEBUG_L1)
    cout << "  dir level: " << dir_level << endl;
  data += sizeof(u_int16_t);
  size -= sizeof(u_int16_t);
  for (int i = 0; i < dir_level; i++)
    {
      // ??
      memcpy(&ukn, data, sizeof(char));
      if (DEBUG_L1)
	cout << "  ?: " << (int)ukn << endl;
      data += sizeof(char);
      size -= sizeof(char);
      // dir name size
      memcpy(&dir_name_size, data, sizeof(u_int16_t));
      dir_name_size = ntohs(dir_name_size);
      if (DEBUG_L1)
	cout << "  dir name size: " << dir_name_size << endl;
      data += sizeof(u_int16_t);
      size -= sizeof(u_int16_t);
      // name
      name = new char[dir_name_size + 1];
      memcpy(name, data, dir_name_size);
      data += dir_name_size;
      size -= dir_name_size;
      name[dir_name_size] = 0;
      if (DEBUG_L1)
	cout << "  dir name: " << name << endl;
      dir.assign(name, dir_name_size);
      if (dir.compare(".."))
	{
	  if (dir.compare("."))
	    dirs.push_back(dir);
	}
      else
	{
	  if (!dirs.empty())
	    dirs.pop_back();
	}
      delete [] name;
      if (DEBUG_L1)
	cout << " Field 202 size left: " << size << endl;
      if (size < 0)
	{
	  cerr << "Field 202 over sized" << endl;
	  dirs.clear();
	  return dirs;
	}
    }
  return dirs;
}

u_int32_t CClServ::Field203(char *data, int size)
{
  char *tmp;
  u_int32_t ret=0;

  tmp = data;
  if (DEBUG_L1)
    {
      cout << " Resume data field - size: " << size << endl;
      print_hexa(data, size);
    }
  tmp = tmp + 46;
  memcpy(&ret, tmp, sizeof(u_int32_t));
  ret = ntohl(ret);
  if (DEBUG_L1)
    cout << " Current file size: " << ret << endl;
  return ret;
} 
