#include "ibaard_network.h"

int netread(int sd, char *buf){
  int i;
#if (defined HAVE_SSL) || (defined HAVE_MATRIXSSL)
  if (am_sslconf & AM_SSL_USETLS){
    i=netsslread(ssl, buf, MAXNETBUF);
  } else 
#endif
    i=recv(sd, buf, MAXNETBUF, 0);
  buf[i]='\0';
  return i;
}
