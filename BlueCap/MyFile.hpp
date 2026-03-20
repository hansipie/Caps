
#include <stdio.h>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <Magick++.h>

using namespace std;
using namespace Magick;

class CMyFile
{
public:
  CMyFile(string);
  ~CMyFile();
  
  string	File;

  bool Existenz();
  bool Move(string);
  bool Remove();
  bool Rename(string);
  bool MkFolder();
  void Make_preview(string);
};
