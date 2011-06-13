#ifndef _IBAARD_H
#define _IBAARD_H

/** @file
 */

#include "ibaard_platform.h"
#include "ibaard_types.h"

#ifdef __dietlibc__
#include <write12.h>
#else
#include <string.h>
#ifdef __WIN32__
#include <io.h>
#endif

#if ((defined __WIN32BORLAND)||(defined __WIN32MSC))
static int __write1(const char *s)
#else
  static inline int __write1(const char *s)
#endif
{
  write(1, s, strlen(s));
  return 0;
}

#if ((defined __WIN32BORLAND)||(defined __WIN32MSC))
static int __write2(const char *s)
#else
  static inline int __write2(const char *s)
#endif
{
  write(2, s, strlen(s));
  return 0;
}
#endif

#if ((defined __WIN32BORLAND)||(defined __WIN32MSC))
static int __writefd(int fd, const char *s)
#else
  static inline int __writefd(int fd, const char *s)
#endif
{
  write(fd, s, strlen(s));
  return 0;
}

/** Split a string
 *
 * @param buf
 * @param c
 * @param len
 * @param plus
 * @param ofs
 * @return @todo
 */
char **split(char *buf,int c,int *len,int plus,int ofs);

/** Draw a nice ascii-art giraffe
 */
void kirahvi();

#endif
