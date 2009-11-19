#include "ibaard_maildir.h"
#include "ibaard_fs.h"
#include "ibaard_log.h"
#include "ibaard_cat.h"
#include "logtypes.h"

int mdfind(char *maildir){
  // TODO: maybe look what's in maildirpath. check if directory exists
  if (maildir) {
    maildirpath=strdup(maildir);
    if (!td(maildirpath)) return 0; 
  } 

  if ((maildirpath=getenv("MAILDIR"))==NULL){
#ifdef __WIN32__
    if (getenv("USERPROFILE")==NULL){
      logmsg(L_ERROR, F_MAILDIR, "%MAILDIR% not set, %USERPROFILE% not found", NULL);
#else
      if (getenv("HOME")==NULL){
        logmsg(L_ERROR, F_MAILDIR, "$MAILDIR not set, $HOME not found", NULL);
#endif
        return -1;
      } else {
#ifdef __WIN32__
        if (cat(&maildirpath, getenv("USERPROFILE"), "/Maildir", NULL)) return -1;
#else
        if (cat(&maildirpath, getenv("HOME"), "/Maildir", NULL)) return -1;
#endif
        else if (!td(maildirpath)) return 0;
      }
    } else {
      maildirpath=strdup(getenv("MAILDIR"));
      if (!td(maildirpath)) return 0;
    }

    return -1; // if we got that far we did not find a usable maildir
  }
