//! CDwlUpl class
/*!
  Class used to bufferize a file for download or upload actions
*/
using namespace std;

#include <iostream>
#include <fstream>
#include <string>

#include "FlFileObj.hpp"

class CDwlUpl
{
public:
  //!The Constructor
  /*!
    The constructor create the stream associated to the file.
    \param _file The file to bufferize.
    \param type stream type, "read" or "write".
  */
  CDwlUpl(string _file, string type);
  //!The Destructor
  /*!
    The destructor close the stream associated to the file.
  */
  ~CDwlUpl();
  
  //! Member moves the stream buffer pointer.
  void MoveTo(u_int32_t offset);
  //! Member that reads a buffer from the stream.
  /*!
    \return Size of the buffer read.
  */
  int GetBuff();  
  //! Member puts a buffer in the stream.
  /*!
    \param rcv Buffer revieved.
    \param size Buffer size.
    \return returns "true" the operation succeded or "false" if not.
  */
  bool PutBuff(char *rcv, int size);
  //! Member clears the buffer in the class.
  void DelBuff();
  //! Menber close the stream associated to the file.
  void CloseFile();
  
  /*!
    File name.
  */
  string	file;
  /*!
    file stream.
  */
  fstream	filestream;
  /*!
    File buffer.
  */
  char		*buffer;
  /*!
    Error flag.
  */
  bool		error;
  /*!
    Delete flag.
  */
  bool		f_delete;
  /*!
    Total read/write size.
  */
  int		total;
};

