#ifndef _WIN32
//TODO: check if we can do something similar on windows
#include <fcntl.h>
#include <sys/stat.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include "ibaard_fs.h"

int mf2(char* name, mode_t mode){
  struct stat fifostat;

  if(stat(name, &fifostat) != -1) {
    if (S_ISFIFO(fifostat.st_mode)){
      // file exists, and is a fifo: adjust mode
      chmod(name, mode);
    } else {
      // file exists, but is not a fifo
      return -1;
    }
  } else { // does not yet exist
    mode_t old_mask=umask(0000);
    int ret=mknod(name, S_IFIFO | mode, 0);
    umask(old_mask);
    if (ret)
      return -1;
  }
  return 0;
}

int mf(char* name){
  mode_t mode=0666;
  mode_t mask=umask(0000);
  umask(mask);

  return mf2(name, mode&~mask);
}
#endif
