/**
 * @file mdinit.c
 * @author Bernd Wachter <bwachter@lart.info>
 * @date 2005-2011
 */

// windows.h is already included in types
// things blow up if it gets included before winsock
#ifndef _WIN32
#include <dirent.h>
#endif
#include <errno.h>

#include "ibaard_maildir.h"
#include "ibaard_fs.h"
#include "ibaard_log.h"
#include "ibaard_cat.h"
#include "logtypes.h"

char *maildirpath;

// FIXME
static int maildir_harddelete=0; // shall we delete the mails, or just mark them deleted and remove them on cleanup?

int mdinit(char *maildir, char *subdir, int harddelete){
  // maybe add a flag to recurse into subdirs
#ifdef _WIN32
  HANDLE dirptr;
  WIN32_FIND_DATA fData;
  char pattern[AM_MAXPATH];
#else
  DIR *dirptr;
  struct dirent *tmpdirent;
#endif
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

#ifdef _WIN32
  snprintf(pattern, "%s\\*.*", mymaildir, AM_MAXPATH);
  dirptr = FindFirstFile(pattern, &fData);

  if (dirptr == INVALID_HANDLE_VALUE){
    // TODO: error message
    free(mymaildir);
    return -1;
  }

  do {
    if (!strncmp(fData.cFileName, ".", 1)) continue;
    if (!strncmp(fData.cFileName, "..", 2)) continue;
  } while(FindNextFile(dirptr, &fData));

  FindClose(dirptr);
#else
  if ((dirptr=opendir(mymaildir))==NULL){
    logmsg(L_ERROR, F_MAILDIR, "unable to open maildir ", mymaildir, ": ", strerror(errno), NULL);
    free(mymaildir);
    return -1;
  }

  for (tmpdirent=readdir(dirptr); tmpdirent!=NULL; tmpdirent=readdir(dirptr)){
    if (!strncmp(tmpdirent->d_name, ".", 1)) continue;
    if (!strncmp(tmpdirent->d_name, "..", 2)) continue;
    if (stat(cati(mymaildir, "/", tmpdirent->d_name, NULL), &maildirstat)==-1){
      logmsg(L_ERROR, F_MAILDIR, "stat() for file in maildir failed ", NULL);
      free(mymaildir);
      return -1;
    }
    strncpy(tmpmaildirent.name, tmpdirent->d_name, AM_MAXPATH);
    tmpmaildirent.size = maildirstat.st_size;
    //maildir_sappend(&tmpmaildirent);
    memset(&tmpmaildirent, 0, sizeof(maildirent));
  }
#endif

  free(mymaildir);
  return 0; //FIXME
}
