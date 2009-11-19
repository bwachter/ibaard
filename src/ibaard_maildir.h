#ifndef _IBAARD_MAILDIR_H
#define _IBAARD_MAILDIR_H
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "ibaard_types.h"

#if (defined(__WIN32__)) || (defined _BROKEN_IO)
#include <stdio.h>
#endif

typedef struct _maildirent maildirent;

struct _maildirent {
    char name[AM_MAXPATH];
    off_t size;
    int deleted;
    maildirent *next;
};

char *maildirpath;

int mduniqname(char **uniqname);
#if (defined(__WIN32__)) || (defined _BROKEN_IO)
FILE *mdopen(char *maildir, char **uniqname);
int mdiclose(char *maildir, char **uniqname, FILE* fd);
#else
int mdopen(char *maildir, char **uniqname);
int mdclose(char *maildir, char **uniqname, int fd);
#endif
int mdinit(char *maildir, char *subdir, int harddelete);
int mdfind(char *maildir);

#endif
