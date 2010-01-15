#include "ibaard_platform.h"
// FIXME, msvc-hack
#ifndef __WIN32MSC

#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include "ibaard_fs.h"

int md2(char* name, mode_t mode) {
  struct stat dirstat;

  if(stat(name, &dirstat) != -1) {
    if (!S_ISDIR(dirstat.st_mode))
      return -1; // exists, but not as a directory
  } else { // does not yet exist
    if
#ifdef __WIN32__
      (mkdir(name))
#else
      (mkdir(name, mode))
#endif
        return -1;
  }
  return 0; // dir either exists or we were able to create it
}

int md(char* name) {
  mode_t mode=0777;
  return md2(name, mode);
}
#endif
