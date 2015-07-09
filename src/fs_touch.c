#include <fcntl.h>
#include <errno.h>
#ifdef _WIN32
#include <stdio.h>
#include <io.h>
#else
#include <unistd.h>
#endif
#include "ibaard_fs.h"

int touch(char *name){
  int fd;
#ifndef _WIN32
  if ((fd=open(name, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR))==-1) return errno;
#else
  if ((fd=open(name, O_CREAT|O_RDWR))==-1) return errno;
#endif
  close(fd);
  return 0;
}
