#if (defined HAVE_SSL) && (!defined HAVE_MATRIXSSL)
#include "ibaard_network.h"

char* strerror_ssl(int err){
  switch (err){
    case SSL_ERROR_NONE: return "No error (WTF are we doing here?)";
    case SSL_ERROR_ZERO_RETURN: return "Connection closed";
    case SSL_ERROR_WANT_READ: return "Operation not complete";
    case SSL_ERROR_WANT_WRITE: return "Operation not complete";
    case SSL_ERROR_WANT_CONNECT: return "Not connected";
    case SSL_ERROR_WANT_ACCEPT: return "Not connected";
    case SSL_ERROR_WANT_X509_LOOKUP: return "x509 lookup failed";
    case SSL_ERROR_SYSCALL: return "I/O error";
    case SSL_ERROR_SSL: return "SSL protocol error";
    default: return "unknown error";
  }
}
#endif
