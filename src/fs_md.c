#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include "ibaard_fs.h"

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
