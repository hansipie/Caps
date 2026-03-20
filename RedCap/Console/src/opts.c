#include "../includes/redcap.h"

/* erreurs d'options */
void    usage()
{
  printf("RedCap -p port\n-h help\n\nDebug mode:\n-f functions\n-v other\n-r read\n-w write\n-t thread\n-u transac\n");
  exit(1);
}

/* parsage des options */
void    pars_opts(int argc, char **argv)
{
  char  opt;
  
  while ((opt = getopt(argc, argv, "p:tuvrfwh")) != -1)
    {
/*       gl_redcap->bridge->port = 0; */
/*       gl_redcap->debug->other = 0; */
/*       gl_redcap->debug->read = 0; */
/*       gl_redcap->debug->functions = 0; */
/*       gl_redcap->debug->write = 0; */
/*       gl_redcap->debug->thread = 1; */
      switch (opt)
        {
        case 'p':
          gl_redcap->bridge->port = atoi(optarg);
          break;
	case 't':
          gl_redcap->debug->thread = 1;
          break;	  
        case 'v':
          gl_redcap->debug->other = 1;
          break;
	case 'r':
          gl_redcap->debug->read = 1;
          break;
	case 'f':
          gl_redcap->debug->functions = 1;
          break;
	case 'u':
          gl_redcap->debug->transac = 1;
          break;
	case 'w':
          gl_redcap->debug->write = 1;
          break;
        case 'h':
          usage();
          break;
        default:
          usage();
        }
    }

}
