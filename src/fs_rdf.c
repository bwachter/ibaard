#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#ifdef __WIN32__
#include <stdio.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#include "ibaard_fs.h"

int rdf(char *name, char **buf){
  int fd, len;
  unsigned long flen=0;

  if ((fd=open(name, O_RDONLY))==-1) return -1;
  if ((buf=malloc(sizeof(char*)*1024))==NULL){
    close(fd);
    return -1;
  }
  while ((len=read(fd,buf,1024))){
    flen+=len;
    if (len>=1024)
      if ((buf=realloc(buf,strlen(*buf)*sizeof(char*)*1024))==NULL){
				close(fd);
				return -1;
      }
  }
  buf[flen]='\0';
	return 0;
}
