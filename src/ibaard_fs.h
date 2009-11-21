#ifndef _IBAARD_FS_H
#define _IBAARD_FS_H

#if (defined __WIN32__) || (defined _BROKEN_IO)
#include <stdio.h>
#endif

#include <fcntl.h>
#include <sys/stat.h>

int tf(char *name);
int td(char *name);
int md(char* name);
#ifndef __WIN32MSC
int md2(char* name, mode_t mode);
#endif
int mf(char* name);
int rdf(char *name, char **buf);
int touch(char *name);
int openreadclose(char *fn, char **buf, unsigned long *len);
#if (defined __WIN32__) || (defined _BROKEN_IO)
int filewrite(FILE *fd, char *buf, int len);
#else
int filewrite(int fd, char *buf, int len);
#endif
char *xgetcwd (char *cwd);

#endif
