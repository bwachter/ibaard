/**
 * @file mdopen.c
 * @author Bernd Wachter <bwachter@lart.info>
 * @date 2005-2011
 */

#include <errno.h>
#include <fcntl.h>

#include "ibaard_maildir.h"
#include "ibaard_log.h"
#include "ibaard_cat.h"
#include "logtypes.h"

// opens a file in maildir/
// if maildir does not start with a / use subdir in maildir
#if (defined(__WIN32__)) || (defined _BROKEN_IO)
FILE *mdopen(char *maildir, char **uniqname){
  FILE *fd;
#else
  int mdopen(char *maildir, char **uniqname){
    int fd;
#endif
    char *path=NULL;

    if ((mdfind(maildir)) == -1) goto errexit;

    mduniqname(uniqname);
    if ((cat(&path, maildirpath, "/tmp/", *uniqname, NULL))) goto errexit;
    logmsg(L_INFO, F_MAILDIR, "spooling to ", path, NULL);
#if (defined(__WIN32__)) || (defined _BROKEN_IO)
    if ((fd=fopen(path, "w+")) == NULL) {
#else
      if ((fd=open(path, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0644)) == -1) {
#endif
        logmsg(L_ERROR, F_MAILDIR, "open ", path, " for writing failed: ", strerror(errno), NULL);
        free(path);
        goto errexit;
      }
      free(path);
      return fd;
      errexit: // with supporting windows-crap / stdio that's easier than many ifdefs
#if (defined(__WIN32__)) || (defined _BROKEN_IO)
      return (FILE*)NULL;
#else
      return -1;
#endif
    }
