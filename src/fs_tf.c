#include <fcntl.h>
#include <errno.h>
#ifdef _WIN32
#include <stdio.h>
#include <io.h>
#else
#include <unistd.h>
#endif
#include "ibaard_fs.h"

int tf(char *name){
  int fd;
  if ((fd=open(name, O_RDONLY))==-1) return errno;
  close(fd);
  return 0;
}
