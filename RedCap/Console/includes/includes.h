#ifdef __i386__
#include <sys/socket.h> /* a desactiver si sous osx */
#endif
#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <stdarg.h>
#include <syslog.h>
#include <sys/wait.h>
#include <math.h>
#include <ctype.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
/* #include "/usr/pkg/include/pthread.h" */
#include <pthread.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/mman.h>
#include <sys/times.h>
