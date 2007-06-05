#ifndef _IBAARD_H
#define _IBAARD_H

#include "ibaard_types.h"

#ifdef __dietlibc__
#include <write12.h>
#else
#include <string.h>
#ifdef __WIN32__
#include <io.h>
#endif

#ifdef __BORLANDC__
static int __write1(const char *s)
#else
static inline int __write1(const char *s)
#endif
{
	write(1, s, strlen(s));
	return 0;
}

#ifdef __BORLANDC__
static int __write2(const char *s)
#else
static inline int __write2(const char *s)
#endif
{
	write(2, s, strlen(s));
	return 0;
}
#endif

#ifdef __BORLANDC__
static int __writefd(int fd, const char *s)
#else
static inline int __writefd(int fd, const char *s)
#endif
{
	write(fd, s, strlen(s));
	return 0;
}

char **split(char *buf,int c,int *len,int plus,int ofs);
void kirahvi();

#endif
