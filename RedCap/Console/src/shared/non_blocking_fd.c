#include "../../includes/redcap.h"

void set_non_blocking ( int m_sock, int b )
{

#ifdef __WIN32__ 
// If opts = 0, blocking is enabled 
// If opts != 0, non-blocking mode is enabled.
    int opts;
    if (b)
        opts = 1; 
    else
        opts = 0;
    ioctlsocket(m_sock, FIONBIO, (u_long FAR*) &opts);
#else
    int opts = fcntl ( m_sock, F_GETFL );
    if ( opts < 0 )
        return;
    if (b)
        opts = ( opts | O_NONBLOCK );
    else
        opts = ( opts & ~O_NONBLOCK );  
    fcntl ( m_sock, F_SETFL, opts );
#endif
}
