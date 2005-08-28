#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#ifdef __WIN32__
#include <stdio.h>
#include <io.h>
#else
#include <unistd.h>
#endif

//#include "aardmail.h"
#include "ibaard_fs.h"


int tf(char *name){
	int fd;
	if ((fd=open(name, O_RDONLY))==-1) return errno;
	close(fd);
	return 0;
}

int td(char *name){
	struct stat dirstat;
	if (!stat(name, &dirstat))
		if (S_ISDIR(dirstat.st_mode)) return 0;
	return -1;
}

int md(char* name) {
	struct stat dirstat;
	mode_t mode=0777;
	
	if(stat(name, &dirstat) != -1) {
		if (!S_ISDIR(dirstat.st_mode)) 
			return -1; // exists, but not as a directory
	} else { // does not yet exist
		if (mkdir(name, mode)) 
			return -1;
	}
	return 0; // dir either exists or we were able to create it
}

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
