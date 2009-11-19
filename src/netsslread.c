#include "ibaard_network.h"

#if (defined HAVE_SSL) && (!defined HAVE_MATRIXSSL)

int netsslread(SSL *ssl_handle, char *buf, int len){
  int i=0;
  while (i<1){
    i=SSL_read(ssl_handle, buf, len);
    switch (SSL_get_error(ssl_handle, i)){
      case SSL_ERROR_WANT_READ:
        continue;
      default:
        return i;
    }
  }
  return i;
}

#endif
