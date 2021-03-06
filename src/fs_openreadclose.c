#include <fcntl.h>
#include <stdlib.h>
#ifdef _WIN32
#include <stdio.h>
#include <io.h>
#else
#include <unistd.h>
#endif
#include "ibaard_fs.h"

int openreadclose(char *fn, char **buf, unsigned long *len) {
  int fd=open(fn,O_RDONLY);
  if (fd<0) return -1;
  if (!*buf) {
    *len=lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    *buf=(char*)malloc(*len+1);
    if (!*buf) {
      close(fd);
      return -1;
    }
  }
  *len=read(fd,*buf,*len);
  if (*len != (unsigned long)-1)
    (*buf)[*len]=0;
  return close(fd);
}
