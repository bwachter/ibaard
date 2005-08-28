#ifndef _IBAARD_H
#define _IBAARD_H

#include "ibaard_types.h"
#include "ibaard_cat.h"
#include "ibaard_fs.h"
#include "ibaard_log.h"
#include "ibaard_strip.h"

#ifdef __dietlibc__
#include <write12.h>
#else
#include <string.h>
#ifdef __WIN32__
#ifdef _GNUC_
#include <getopt.h>
#else
#include "getopt.h"
#endif
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

char **split(char *buf,int c,int *len,int plus,int ofs);

#endif
