#include "Hotline.hpp"
#include "CDwlUpl.hpp"


CDwlUpl::CDwlUpl(string _file, string type)
{
  //cout << "Constructeur CDwlUpl." << endl;
  file.assign(_file);
  //cout << "File: " << file << endl;
  if (type == "read")
    {
      //cout << " openning stream for reading" << endl;
      filestream.open(file.c_str(), ios::in|ios::binary);
    }
  else
    {
      //cout << " openning stream for writing" << endl;
      filestream.open(file.c_str(), ios::out|ios::trunc|ios::binary);
    }
  if (!filestream.is_open())
    {
      cerr << "CDwlUpl::CDwlUpl() : Error openning file" <<endl;
      error = true;
      return;
    }
  error = false;
  f_delete = false;
  if (type == "read")
    filestream.seekg (0, ios::beg);
  total = 0;
}

CDwlUpl::~CDwlUpl()
{
  //cout << "Destroying CDwlUpl... ";
  CloseFile();
}

void CDwlUpl::CloseFile()
{
  //cout << "closing stream ..." << endl;
  filestream.close();
}

void CDwlUpl::MoveTo(u_int32_t offset)
{
  filestream.seekg (offset, ios::beg);
}

int CDwlUpl::GetBuff()
{
  if (!filestream.is_open())
    {
      cerr << "CDwlUpl::GetBuff() : error openning file" <<endl;
      error = true;
      return -1;
    }
  buffer = new char [BUFFSIZE];
  filestream.read(buffer, BUFFSIZE);
  return filestream.gcount();
}

bool CDwlUpl::PutBuff(char *rcv, int size)
{
  if (!filestream.is_open())
    {
      cerr << "CDwlUpl::PutBuff() : error openning file" <<endl;
      error = true;
      return false;
    }
  filestream.flush();
  filestream.write(rcv, size);
  return true;
}

void CDwlUpl::DelBuff()
{
  delete [] buffer;
}
