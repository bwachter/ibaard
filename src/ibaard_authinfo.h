#ifndef _AUTHINFO_H
#define _AUTHINFO_H

/** @file
 * Functions for authinfo parsing
 */

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netdb.h>
#endif

#include "ibaard_types.h"

/** An entry in .authinfo */
typedef struct _authinfo authinfo;

/** A key in authinfo, with information if it has arguments */
typedef struct _authinfo_key authinfo_key;

struct _authinfo {
    char machine[NI_MAXHOST];
    char port[NI_MAXSERV];
    char login[AM_MAXUSER];
    char password[AM_MAXPASS];
    int defaultauth;
    int force;
    authinfo *next;
};

struct _authinfo_key {
    char *name;
    int hasargs;
};

/** Parse authinfo
 *
 * @todo complete documentation
 * @return -1 on error, 0 on success
 */
int authinfo_init();

/** Look up an authinfo record
 *
 * @todo complete documentation
 */
int authinfo_lookup(authinfo *authinfo_data);

#endif
