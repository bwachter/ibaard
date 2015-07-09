/**
 * @file mdclose.c
 * @author Bernd Wachter <bwachter@lart.info>
 * @date 2005-2011
 */

#include <errno.h>

#include "ibaard_log.h"
#include "ibaard_maildir.h"
#include "ibaard_cat.h"
#include "logtypes.h"

#if (defined(_WIN32)) || (defined _BROKEN_IO)
int mdclose(char *maildir, char **uniqname, FILE *fd){
#else
  int mdclose(char *maildir, char **uniqname, int fd){
#endif
    char *oldpath=NULL, *newpath=NULL;
    int status=0;
    if ((mdfind(maildir)) == -1) {
#if (defined(_WIN32)) || (defined _BROKEN_IO)
      fflush(fd);
      fclose(fd);
#else
      close(fd);
#endif
      return -1;
    }

    cat(&oldpath, maildirpath, "/tmp/", *uniqname, NULL);
    cat(&newpath, maildirpath, "/new/", *uniqname, NULL);
    //cat(&newpath, maildirpath, "/cur/", *uniqname, ":2,", NULL);

#if (defined(_WIN32)) || (defined _BROKEN_IO)
    if (!(status=fclose(fd))){
#else
      if (!(status=close(fd))){
#endif
#ifdef _WIN32
        status=MoveFile(oldpath, newpath);
#else
        status=link(oldpath, newpath);
        unlink(oldpath);
#endif
      } else logmsg(L_ERROR, F_GENERAL, "Closing mail failed: ", strerror(errno), NULL);

      free(oldpath);
      free(newpath);
      return status;
    }
