#ifndef _IBAARD_MAILDIR_H
#define _IBAARD_MAILDIR_H
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

/** @file
 * Functions for handling Maildirs
 */

#include "ibaard_types.h"

#if (defined(_WIN32)) || (defined _BROKEN_IO)
#include <stdio.h>
#endif

/** An entry in a Maildir */
typedef struct _maildirent maildirent;

struct _maildirent {
    char name[AM_MAXPATH];
    off_t size;
    int deleted;
    maildirent *next;
};


/** The path to the Maildir, once found */
char *maildirpath;

/** Create a unique name to store a mail in Maildir
 *
 * @todo complete documentation
 */
int mduniqname(char **uniqname);


#if (defined(_WIN32)) || (defined _BROKEN_IO)
FILE *mdopen(char *maildir, char **uniqname);
int mdclose(char *maildir, char **uniqname, FILE* fd);
#else
/** Open a file in Maildir for writing
 *
 * Create a file in Maildir/tmp. Use mdclose() to close the file
 *
 * @todo complete documentation
 */
int mdopen(char *maildir, char **uniqname);

/** Close a file in Maildir, opened by mdopen()
 *
 * This will move the file from tmp/ to new/
 *
 * @todo complete documentation
 */
int mdclose(char *maildir, char **uniqname, int fd);
#endif

/** Prepare maildir for RW-Access
 *
 * @todo function is incomplete
 * @todo complete documentation
 */
int mdinit(char *maildir, char *subdir, int harddelete);

/** Try to locate the users Mailder
 *
 * @todo complete documentation
 */
int mdfind(char *maildir);

#endif
