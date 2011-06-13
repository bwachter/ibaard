/*
 * fs_wf.c
 * (c) 2011 Bernd Wachter <bwachter@lart.info>
 */

#include <fcntl.h>
#include <errno.h>
#include <string.h>
#ifdef __WIN32__
#include <stdio.h>
#include <io.h>
#else
#include <unistd.h>
#endif
#include "ibaard_fs.h"

int wf(char *name, char *content){
  int fd;
  int len;
  if ((fd=open(name, O_CREAT|O_TRUNC|O_WRONLY, S_IWUSR|S_IRUSR))==-1) return errno;
  len=write(fd, content, strlen(content));
  close(fd);
  return len;
}