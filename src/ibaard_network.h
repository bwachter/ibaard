#ifndef _IBAARD_NETWORK_H
#define _IBAARD_NETWORK_H

/** @file
 * Network functions and types
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#ifdef __WIN32__
#include <winsock2.h>
#include <windows.h>
#include <winbase.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

#include "ibaard_types.h"

/** Maximum buffer size for network functions
 * @todo: rework network functions to work with arbitrary buffer sizes
 *
 */
#define MAXNETBUF 1024

/** Constants to specify use of SSL-encrypted connections */
enum AM_SSL {
  /// Allow fallback to unencrypted connection
  AM_SSL_ALLOWPLAIN=1,
  /// Use TLS
  AM_SSL_USETLS=2,
  /// Use STARTTLS
  AM_SSL_STARTTLS=4,
};

#ifdef __WIN32__
#define EAI_ADDRFAMILY   -9    /* Address family for NAME not supported.  */
#define EAI_SYSTEM       -11   /* System error returned in `errno'.  */
#endif

#if (defined HAVE_SSL) || (defined HAVE_MATRIXSSL)
extern int am_sslconf;
extern int am_ssl_paranoid;
extern char am_sslkey[1024];
extern char am_ssl_servercerts[1024];
#endif

#ifdef HAVE_SSL
#include <openssl/ssl.h>
extern SSL *ssl;
#endif

#ifdef HAVE_MATRIXSSL
#include <matrixSsl.h>
typedef struct {
    ssl_t *ssl;
    sslBuf_t inbuf;
    sslBuf_t insock;
    sslBuf_t outsock;
    int outBufferCount;
    int fd;
} SSL; // was: sslConn_t
SSL *ssl;
#endif

/** A wrapper around getaddrinfo() when available, emulation on other platforms
 *
 * netaddrinfo() is just a wrapper around getaddrinfo() on systems which
 * support getaddrinfo(). On systems which don't support getaddrinfo it gets
 * emulated using traditional socket functions. On Windows it first tries to
 * load getaddrinfo() from DLL, and falls back to the emulation code at runtime
 * if required.
 *
 * @note for detailed information consult the getaddrinfo() documentation
 *
 * @param node internet host name
 * @param service service name
 * @param hints addrinfo structure with pointers for selecting socket
 *        address structures
 * @param res a list of address structures after lookup
 * @return 0 on success, different errors on failure. Consult the getaddrinfo
 *         documentation for possible error codes
 */
int netaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);

/** A wrapper around freeaddrinfo() when available, emulation on other platforms
 *
 * @todo complete documentation
 */
void netfreeaddrinfo (struct addrinfo *ai);

/** Opens a socket to the host:service given as arguments and returns the socked file descriptor.
 * @todo complete documentation
 */
int netconnect(char *hostname, char *servicename);

/** Opens a socket to the host:service given as arguments and returns the socked file descriptor.
 * @todo complete documentation
 */
int netconnect2(char *hostname, char *servicename, char *bindaddress);

/** Read data from a file descriptor
 *
 * netread() reads up to MAXNETBUF bytes from a given file descriptor and
 * returns the number of bytes read. The read bytes are stored (NULL-
 * terminated) in the buffer specified by buf. You need to make sure that
 * buf has enough space for MAXNETBUF + 1 bytes.
 *
 * netread(), as well as the the functions netreadline() and netwriteline()
 * can handle both plaintext and SSL. If there's an open SSL connection
 * they'll use SSL, if not they'll read/write plaintext.
 *
 * @todo complete documentation
 */
int netread(int sd, char *buf);

/** Read data from a file descriptor, up to \\r\\n
 *
 * netreadline() reads up to MAXNETBUF bytes from a given file descriptor.
 * It stops if either \\r\\n or a NULL byte occurs. It returns the number of
 * bytes read (and stored). Since strlen() can't cope with NULL bytes (it
 * will take the first NULL byte as end of string) always use the return
 * value, never use strlen() on the returned string.
 *
 * @todo complete documentation
 */
int netreadline(int sd, char *buf);
//int netwrite(int sd, char *buf);

/** Write buffer contents to file descriptor
 *
 * netwriteline() writes the content of the buffer pointed to by buf to a
 * given file descriptor. Since netwriteline() needs to find the end of the
 * buffer no NULL bytes (except for the terminating one) are allowed in the
 * input. It returns the number of bytes written.
 *
 * @todo complete documentation
 */
int netwriteline(int sd, char *buf);

/** Dump hostname and service to log
 *
 * Mainly usefol for debug reasons
 *
 * @todo complete documentation
 */
void netlogportservice(const struct sockaddr *sa, socklen_t salen, char *msg);

/** A wrapper around getnameinfo() when available, emulation on other platforms
 *
 * netnameinfo() is just a wrapper around getnameinfo() on systems which
 * support it, just like netaddrinfo().
 *
 * @todo complete documentation (parameters, return value)
 */
int netnameinfo(const struct sockaddr *sa, socklen_t salen,
                char *hostname, size_t hostlen,
                char *servname, size_t servlen, int flags);

/** Open a network socket
 *
 * Used by netconnect()
 *
 * @todo complete documentation
 */
int netsocket(struct addrinfo *ai);

/** Open a network socket and bind to specific address
 *
 * Used by netconnect()
 *
 * @todo complete documentation
 */
int netsocket2(struct addrinfo *ai, struct addrinfo *lai);

#if (defined HAVE_SSL) || (defined HAVE_MATRIXSSL)
/** Negotiate a SSL session on a given file descriptor
 *
 * @note You can obtain the file descriptor from netconnect()
 * @todo complete documentation
 */
int netsslstart(int sd);
char* strerror_ssl(int err);
#endif
#ifdef HAVE_SSL
/** Read input from SSL handle
 *
 * netsslread() reads up to len bytes from a given SSL handle and stores
 * them in the buffer pointed to by buf. You usually don't need this function
 * -- netreadline() handles both encrypted and unencrypted connections.
 *
 * netsslread() and netsslwrite() can cope with a certificate request from
 * the server. If you have set a certificate (global variable am_sslkey keeps
 * the pathname to the key) they will try to re-negotiate the handshake using
 * your certificate once the server requests it
 *
 * @todo complete documentation
*/
int netsslread(SSL *ssl_handle, char *buf, int len);

/** Write data to SSL handle
 *
 * netsslwrite() writes up to len bytes from the buffer pointed to by buf to a
 * given SSL handle. You usually don't need this function -- netwriteline()
 * handles both entcrypted and unencrypted connections.
 *
 * @todo complete documentation
 */
int netsslwrite(SSL *ssl_handle, char *buf, int len);
int netsslcacert(char *subdir);
#endif
#ifdef HAVE_MATRIXSSL
int netsslread(sslConn_t *ssl_handle, char *buf, int len);
int netsslwrite(sslConn_t *ssl_handle, char *buf, int len);
#endif
#endif
