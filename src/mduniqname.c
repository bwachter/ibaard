/**
 * @file mduniqname.c
 * @author Bernd Wachter <bwachter@lart.info>
 * @date 2005-2011
 */

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#ifdef _POSIX_SOURCE
#include <sys/time.h>
#endif
#include "ibaard_maildir.h"
#include "ibaard_log.h"
#include "ibaard_cat.h"
#include "logtypes.h"

static int deliveries=0;

int mduniqname(char **uniqname){
  char tmpbuf[512];
  char myhost[NI_MAXHOST];
  pid_t mypid=getpid();
#ifndef _POSIX_SOURCE
  time_t mytime=time(NULL);
#else
  struct timeval mytime;
  gettimeofday(&mytime, NULL);
#endif

  if (gethostname(myhost, NI_MAXHOST)==-1){
    logmsg(L_WARNING, F_MAILDIR, "unable to get hostname, setting to localhost.localdomain", NULL);
    strcpy(myhost, "localhost.localdomain");
  }

  deliveries++;

#ifdef _POSIX_SOURCE
  sprintf(tmpbuf, "%li.M%liP%iQ%i", (unsigned long)mytime.tv_sec, (unsigned long)mytime.tv_usec, (int) mypid, deliveries);
#else
  sprintf(tmpbuf, "%li.P%iQ%i", (unsigned long)mytime, (int) mypid, deliveries);
#endif
  cat(&*uniqname, tmpbuf, ".", myhost, NULL);

  return 0; // FIXME, kludge
}
