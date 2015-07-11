#include "ibaard_platform.h"
#include <fcntl.h>
#ifndef _WIN32
#include <dirent.h>
#endif
#include <sys/stat.h>
#include "ibaard_fs.h"

#ifndef _WIN32
int md2(char* name, mode_t mode) {
  struct stat dirstat;

  if(stat(name, &dirstat) != -1) {
    if (!S_ISDIR(dirstat.st_mode))
      return -1; // exists, but not as a directory
  } else { // does not yet exist
    if (mkdir(name, mode))
        return -1;
  }
  return 0; // dir either exists or we were able to create it
}
#endif

int md(char* name) {
#ifdef _WIN32
  return mkdir(name);
#else
  mode_t mode=0777;
  return md2(name, mode);
#endif
}
