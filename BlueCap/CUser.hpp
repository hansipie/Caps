//! CUser class
/*!
  Class used to define a user in the server
*/

using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "Systypes.hpp"

class CUser
{
public:
  //!The Constructor
  /*!
    The constructor creates an empty user using his connection's fd
    \param _fd file descriptor.
  */
  CUser(int _fd);
  //!The Destructor
  ~CUser();

  //! Member that serializes the class in the file given as an argument.
  /*!
    \param path Serialisation file path.
  */
  void Serialize(string path);
 
  /*! 
    Tells if the client associated to this class is connected (true) or not (false).
  */
  bool connected;
  /*!
    Gives the connection fd of the client associated to this class.
  */
  int fd;
  /*!
    Gives the name of the client associated to this class. This value can not be changed by the client, it is given by the account file.
  */
  string confname;
  /*!
    Gives the login of the client associated to this class. This value can not be changed by the client, it is given by the account file.
  */
  string login;
  /*!
    Gives the password of the client associated to this class. This value can not be changed by the client, it is given by the account file.
  */
  string password;
  /*!
    Gives the user id of the client associated to this class. This value is set by a random operation.
  */ 
  u_int16_t userId;
  /*!
    Gives the name of the client associated to this class. This value is set by the client.
  */
  string name; 
  /*!
    Gives the icon id of the client associated to this class. This value is set by the client.
  */  
  u_int16_t iconId;
  /*!
    Gives the version of the client associated to this class. This value is set be the client.
  */
  u_int16_t version;
  /*!
    Gives the user flag of the client associated to this class. This value is set be the client.
  */
  u_int16_t userFlags;
  /*!
    Translation as booleans of the user flag.
  */
  bool isaway, isadmin, rpm, rpc, ar;
  /*!
    Auto response sent when the ar flag is activate.
  */
  string auto_rep;
  /*!
    Gives the user access of the client associated to this class, it a array of booleans. 
    These values can not be changed by the client, they are given by the account file.
  */
  bool access[41];
  /*!
    List of downloads in progress.
   */
  map<string, float> download;
  /*!
    List of uploads in progress.
   */
  map<string, float> upload;
};

