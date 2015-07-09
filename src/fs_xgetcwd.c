/*
 * xgetcwd.c -- return current directory with unlimited length
 * Copyright (C) 1992, 1996 Free Software Foundation, Inc.
 * Written by David MacKenzie <djm@gnu.ai.mit.edu>.
 *
 * Special function for busybox written by Vladimir Oleynik <vodz@usa.net>
 */

#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#ifdef _WIN32
#include <stdio.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/param.h>
#endif

#include "ibaard_fs.h"

char *xgetcwd (char *cwd)
{
  char *ret;
  unsigned path_max = 256;

  if(cwd==0)
    cwd = malloc (path_max);

  while ((ret = getcwd (cwd, path_max)) == NULL && errno == ERANGE) {
    path_max += 32;
    cwd = realloc (cwd, path_max);
  }

  if (ret == NULL) {
    // errmsg(errno);
    free (cwd);
    return NULL;
  }
  return cwd;
}
