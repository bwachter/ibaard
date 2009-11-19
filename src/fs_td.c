#include <sys/stat.h>
#include "ibaard_fs.h"

int td(char *name){
  struct stat dirstat;
  if (!stat(name, &dirstat))
    if (S_ISDIR(dirstat.st_mode)) return 0;
  return -1;
}
