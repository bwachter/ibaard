#include <dirent.h>
#include <errno.h>

#include "ibaard_maildir.h"
#include "ibaard_fs.h"
#include "ibaard_log.h"
#include "ibaard_cat.h"
#include "logtypes.h"

// FIXME
static int maildir_harddelete=0; // shall we delete the mails, or just mark them deleted and remove them on cleanup?

int mdinit(char *maildir, char *subdir, int harddelete){
	// maybe add a flag to recurse into subdirs
	DIR *dirptr;
	struct dirent *tmpdirent;
	char *mymaildir=NULL;
	maildirent tmpmaildirent;
	struct stat maildirstat;

	maildir_harddelete = harddelete;
	memset(&maildirstat, 0, sizeof(struct stat));
	memset(&tmpmaildirent, 0, sizeof(maildirent));
	
	if (mdfind(maildir)){
		logmsg(L_ERROR, F_MAILDIR, "unable to find maildir", NULL);
		return -1;
	}

	if (subdir != NULL) cat(&mymaildir, maildirpath, "/", subdir, "/new", NULL);
	else cat(&mymaildir, maildirpath, "/new", NULL);

	if ((dirptr=opendir(mymaildir))==NULL){
		logmsg(L_ERROR, F_MAILDIR, "unable to open maildir ", mymaildir, ": ", strerror(errno), NULL);
		return -1;
	}

	for (tmpdirent=readdir(dirptr); tmpdirent!=NULL; tmpdirent=readdir(dirptr)){
		if (!strncmp(tmpdirent->d_name, ".", 1)) continue;
		if (!strncmp(tmpdirent->d_name, "..", 2)) continue;
		if (stat(cati(mymaildir, "/", tmpdirent->d_name, NULL), &maildirstat)==-1){
			logmsg(L_ERROR, F_MAILDIR, "stat() for file in maildir failed ", NULL);
			return -1;
		}
		strncpy(tmpmaildirent.name, tmpdirent->d_name, AM_MAXPATH);
		tmpmaildirent.size = maildirstat.st_size;
		//maildir_sappend(&tmpmaildirent);
		memset(&tmpmaildirent, 0, sizeof(maildirent));
	}
	return 0; //FIXME
}
