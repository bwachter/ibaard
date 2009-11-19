#include <fcntl.h>
#include <sys/stat.h>
#ifndef __WIN32__
#include <unistd.h>
#endif
#include "ibaard_fs.h"

int mf(char* name){
  struct stat fifostat;
  mode_t mode=0666;
  if(stat(name, &fifostat) != -1) {
    // do some crap
  } else { // does not yet exist
    if (mknod(name, S_IFIFO | mode, 0))
      return -1;
  }
  return 0;
}
