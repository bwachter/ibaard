#include <fcntl.h>
#include <sys/stat.h>
#ifndef __WIN32__
#include <unistd.h>
#endif
#include "ibaard_fs.h"

int mf2(char* name, mode_t mode){
  struct stat fifostat;

  if(stat(name, &fifostat) != -1) {
    // do some crap
  } else { // does not yet exist
    if (mknod(name, S_IFIFO | mode, 0))
      return -1;
  }
  return 0;
}

int mf(char* name){
  mode_t mode=0666;

  return mf2(name, mode);
}
