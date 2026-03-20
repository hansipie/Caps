#include "MyFile.hpp"

CMyFile::CMyFile(string _file)
{
  File = _file;
  //cout << "Acion on " << File << endl;
}

CMyFile::~CMyFile()
{
}

bool CMyFile::Existenz()
{
  FILE *pFile;
  pFile = fopen (File.c_str(),"r");
  if (pFile!=NULL)
    {
      //cout << "file [" << File << "] exist." << endl;
      fclose (pFile);
      return true;
    }        
  //cout << "file [" << File << "] not found." << endl;
  return false;
}    

bool CMyFile::Remove()
{
  //cout << "Removing " << File <<endl;
  if( remove(File.c_str()) == -1 )
    {
      perror("Error removing file");
      return (false);
    }
  //cout << "File successfully removed" << endl;
  return (true);
}

bool CMyFile::Rename(string newname)
{
  int result;
  //cout << "Renaming/Moving " << File << " to " << newname << endl;
  result= rename(File.c_str(), newname.c_str());
  if (result != 0 )
    {
      perror( "Error renaming/moving file" );
      return (false);
    }
  //cout << "File successfully renamed/moved" << endl;
  File = newname;
  return (true);
}

bool CMyFile::Move(string newname)
{
  return (Rename(newname));
}

bool CMyFile::MkFolder()
{
  //cout << "Creating folder "<< File;
  
#ifndef __WIN32__
  if (mkdir(File.c_str(), 0) == -1)
    {
      //cout << " ... failed" << endl;
      perror("Error creating folder");
      return false;
    }
  //cout << " ... ok" << endl;
  return true;
#else
  //cout << "... doesn't work under win32" << endl;
  return false;       
#endif    
}

void CMyFile::Make_preview(string ext)
{
  //cout << "Making preview ..." << endl;
  string File_save(File);  
  File.append("_tmp"+ext);
  try
    {
      Image image(File_save); 
      image.scale("320x240");
      image.write(File);
    }
  catch(Exception &error_)
    { 
      cerr << "Caught exception: " << error_.what() << endl;
    }
}
