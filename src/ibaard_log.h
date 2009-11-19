#ifndef _IBAARD_LOG_H
#define _IBAARD_LOG_H

/*
  F_GENERAL,
  F_NET,
  F_SSL,
  F_FS,
  F_MAILDIR,
  F_AUTHINFO,
  F_ADDRLIST,
*/

enum loglevels {
  L_UNSPEC,
  L_DEADLY,
  L_ERROR,
  L_WARNING,
  L_INFO,
  L_VERBOSE,
  L_DEBUG,
};

int logmsg(int level, char *facility, char *msg, ...);
int logmsg_fd(int fd, int level, char *facility, char *msg, ...); 
int loglevel(int level);
int lmsg(char *msg, ...);
int emsg(char *msg, ...);
int dmsg(char *msg, ...);

#endif
