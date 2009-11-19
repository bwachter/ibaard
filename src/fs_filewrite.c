#ifdef __WIN32__
#include <stdio.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#include "ibaard_fs.h"

#if (defined __WIN32__) || (defined _BROKEN_IO)
int filewrite(FILE *fd, char *buf, int len)
#else
  int filewrite(int fd, char *buf, int len)
#endif
{
#if (defined __WIN32__) || (defined _BROKEN_IO)
  return fwrite(buf, 1, len, fd);
#else
  return write(fd, buf, len);
#endif
}
