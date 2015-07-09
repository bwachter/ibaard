#ifndef _AM_TYPES_H
#define _AM_TYPES_H

/** @file
 * Type definitions, mainly for non-POSIX platforms to make people
 * used to POSIX feel at home
 */

#ifdef _WIN32
// the great win32 sdk needs winsock2.h to be included before windows.h...
#include <winsock2.h>
#include <io.h>
#include <windows.h>
#include <winbase.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
//#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#if (defined(sun)) || (defined (sgi))
#include <inttypes.h>
#else
#include <stdint.h>
#endif
#endif

#ifdef _WIN32
#ifndef strcasecmp
#define strcasecmp(a,b) stricmp(a,b)
#endif
#ifndef strncasecmp
#define strncasecmp(a,b,c) strnicmp(a,b,c)
#endif
#endif

/// Maximum length of username (introduced for aardmail)
#define AM_MAXUSER 1025
/// Maximum length of password (introduced for aardmail)
#define AM_MAXPASS 1025
/// Maximum path name length (introduced for aardmail)
#define AM_MAXPATH 1025

#ifdef __BORLANDC__
#define pclose(a) _pclose(a)
#define popen(a,b) _popen(a,b)
struct addrinfo {
    int     ai_flags;
    int     ai_family;
    int     ai_socktype;
    int     ai_protocol;
    size_t  ai_addrlen;
    struct sockaddr *ai_addr;
    char   *ai_canonname;
    struct addrinfo *ai_next;
};

typedef unsigned __int32 uint32_t;
typedef unsigned __int16 uint16_t;
typedef uint32_t socklen_t;
typedef int pid_t;
#endif

#ifndef NI_MAXHOST
/// Maximum host name length
#define NI_MAXHOST 1025
#endif
#ifndef NI_MAXSERV
/// Maximum service name length
#define NI_MAXSERV 32
#endif
#ifndef NI_NUMERICSERV
/// Return port number instead of service name
#define NI_NUMERICSERV 8
#endif
#ifndef NI_NUMERICHOST
/// Return numeric host instead of host name
#define NI_NUMERICHOST 2
#endif

#ifndef EAI_FAMILY
/// Address family not recognized or address length invalid for family
#define EAI_FAMILY -1
#endif
#ifndef EAI_NONAME
/// Name does not resolve
#define EAI_NONAME -4
#endif
#ifndef EAI_SERVICE
/// Service not supported for this socket type
#define EAI_SERVICE -5
#endif
#ifndef EAI_NODATA
/// Deprecated years ago, you should use EAI_NONAME
#define EAI_NODATA EAI_NONAME
#endif
#ifndef EAI_MEMORY
/// Memory allocation failure
#define EAI_MEMORY -8
#endif
#ifndef EAI_FAIL
/// Some unspecific non-recoverable error
#define EAI_FAIL -9
#endif
#ifndef EAI_AGAIN
/// Could not resolve name, better luck next time
#define EAI_AGAIN -10
#endif

// socket stuff
#ifndef AI_NUMERICHOST
/// Don't resolve names -> you need to specify a numeric hostname
#define AI_NUMERICHOST 1
#endif
#ifndef AI_CANONNAME
/// When set ai_canonname of first addrinfo struct will point to canonical name
#define AI_CANONNAME 2
#endif
#ifndef AI_PASSIVE
/// Set this if you want to use the address structure for bind()
#define AI_PASSIVE 4
#endif

#endif
