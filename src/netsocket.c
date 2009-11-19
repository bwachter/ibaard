#include "ibaard_network.h"
#include "ibaard_types.h"
#include "ibaard_log.h"
#include "logtypes.h"

int netsocket(struct addrinfo *ai){
  return netsocket2(ai, 0);
}

int netsocket2(struct addrinfo *ai, struct addrinfo *lai){
  int sd;
#if (defined HAVE_SSL) || (defined HAVE_MATRIXSSL)
  int err;
#endif

  if (loglevel(0) > 0)
    netlogportservice(ai->ai_addr, ai->ai_addrlen, "Trying to connect to: ");

  if ((sd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0){
    logmsg(L_ERROR, F_NET, "socket() failed: ", strerror(errno), NULL);
    return -1;
  }

  if (lai!=0)
    if (bind(sd, lai->ai_addr, lai->ai_addrlen) < 0){
      logmsg(L_ERROR, F_NET, "bind() failed: ", strerror(errno), NULL);
    }

  if (connect(sd, ai->ai_addr, ai->ai_addrlen) < 0){
    logmsg(L_ERROR, F_NET, "connect() failed: ", strerror(errno), NULL);
    return -1;
  }

#if (defined HAVE_SSL) || (defined HAVE_MATRIXSSL)
  if (am_sslconf & AM_SSL_USETLS){
    if ((err=netsslstart(sd)) && (am_sslconf & AM_SSL_ALLOWPLAIN)){
      logmsg(L_WARNING, F_NET, "no ssl available, continuing from start", NULL);
      close(sd);
      return netsocket(ai);
    } else if (err && !(am_sslconf & AM_SSL_ALLOWPLAIN)){
      logmsg(L_DEADLY, F_NET, "unable to open ssl connection, plaintext fallback disabled.", NULL);
    }
  }
#endif
  return sd;
}
