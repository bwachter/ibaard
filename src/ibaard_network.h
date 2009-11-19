#ifndef _IBAARD_NETWORK_H
#define _IBAARD_NETWORK_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#ifdef __WIN32__
#include <windows.h>
#include <winbase.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

#include "ibaard_types.h"

#define MAXNETBUF 1024
#define AM_SSL_ALLOWPLAIN 1
#define AM_SSL_USETLS 2
#define AM_SSL_STARTTLS 4

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

int netaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
int netconnect(char *hostname, char *servicename);
// transport binding address
int netconnect2(char *hostname, char *servicename, char *bindaddress);
int netread(int sd, char *buf);
int netreadline(int sd, char *buf);
//int netwrite(int sd, char *buf);
int netwriteline(int sd, char *buf);
void netlogportservice(const struct sockaddr *sa, socklen_t salen, char *msg);
int netnameinfo(const struct sockaddr *sa, socklen_t salen, 
                char *hostname, size_t hostlen, 
                char *servname, size_t servlen, int flags);
int netsocket(struct addrinfo *ai);
// connect and bind to local address
int netsocket2(struct addrinfo *ai, struct addrinfo *lai);

#if (defined HAVE_SSL) || (defined HAVE_MATRIXSSL)
int netsslstart(int sd);
char* strerror_ssl(int err);
#endif
#ifdef HAVE_SSL
int netsslread(SSL *ssl_handle, char *buf, int len);
int netsslwrite(SSL *ssl_handle, char *buf, int len);
int netsslcacert(char *subdir);
#endif
#ifdef HAVE_MATRIXSSL
int netsslread(sslConn_t *ssl_handle, char *buf, int len);
int netsslwrite(sslConn_t *ssl_handle, char *buf, int len);
#endif
#endif
