/*
 * fs_wf.c
 * (c) 2011 Bernd Wachter <bwachter@lart.info>
 */

#include <fcntl.h>
#include <errno.h>
#include <string.h>
#ifdef _WIN32
#include <stdio.h>
#include <io.h>
#else
#include <unistd.h>
#endif
#include "ibaard_fs.h"

int wf(char *name, char *content){
  int fd;
  int len;
#ifdef _WIN32
  if ((fd=open(name, O_CREAT|O_TRUNC|O_WRONLY))==-1) return errno;
#else
  if ((fd=open(name, O_CREAT|O_TRUNC|O_WRONLY, S_IWUSR|S_IRUSR))==-1) return errno;
#endif
  len=write(fd, content, strlen(content));
  close(fd);
  return len;
}
