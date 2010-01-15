#include <stdint.h>
#include "ibaard_network.h"
#include "ibaard_log.h"
#include "logtypes.h"

int netnameinfo(const struct sockaddr *sa, socklen_t salen,
                char *hostname, size_t hostlen,
                char *servname, size_t servlen, int flags){
#ifdef __WIN32__
  HINSTANCE _hInstance = LoadLibrary( "ws2_32" );
  int (WSAAPI *pfn_getnameinfo) (const struct sockaddr*, socklen_t salen,
                                 char *hostname, size_t hostlen,
                                 char *servname, size_t servlen, int flags);

  pfn_getnameinfo =     GetProcAddress( _hInstance, "getnameinfo" );

  if (pfn_getnameinfo){
    return (pfn_getnameinfo(sa, salen, hostname, hostlen, servname, servlen, flags));
  } else {
#endif
#if (defined( __WIN32__)) || (defined(_BROKEN_IO))
#ifdef __GNUC__
    (void) flags;
    (void) salen;
#endif
    char *tmp;
    if ((tmp = malloc((NI_MAXHOST+1)*sizeof(char))) == NULL) {
      logmsg(L_ERROR, F_NET, "malloc() failed", NULL);
      return -1;
    }

    if (servname != NULL) {
      uint16_t service_int;
      service_int = ntohs(((struct sockaddr_in*)sa)->sin_port);
      snprintf(tmp, NI_MAXHOST, "%i", service_int);
      strncpy(servname, tmp, servlen);
    }

    free(tmp);

    if (hostname != NULL) {
      if ((tmp = inet_ntoa(((struct sockaddr_in*)sa)->sin_addr)) == NULL){
        logmsg(L_ERROR, F_NET, "converting ip failed", NULL);
      }
      strncpy(hostname, tmp, hostlen);
    }
    return 0;
#ifdef __WIN32__
  }
#endif
#else
  return (getnameinfo(sa, salen, hostname, hostlen, servname, servlen, flags));
#endif
}
