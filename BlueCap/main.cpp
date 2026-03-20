#include "Serv.hpp"

int main(int argc, char **argv, char **env)
{  
#ifdef __WIN32__
  //InitializeMagick(*argv);
#endif
  CServ mylink;
  mylink.env = env;
  mylink.LoopZ();
  return(0);
}
