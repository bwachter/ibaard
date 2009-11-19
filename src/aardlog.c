#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#ifndef __WIN32__
#include <strings.h>
#endif

#include "ibaard.h"
#include "ibaard_log.h"

static int current_loglevel=L_ERROR;

static void logwrite (int fd, char *msg){
  char *tmp = strdup(msg);

  if (!strncasecmp(tmp+strlen(tmp)-2, "\r\n", 2))
    tmp[strlen(tmp)-2]='\0';
  __writefd(fd, tmp);
  free(tmp);
  return;
}

int loglevel(int level){
  if (level != 0)
    current_loglevel = level;
  return current_loglevel; 
}


static int logheader(int fd, int level, char *facility){
  int die=0;

  switch(level){
    case L_DEADLY:
      __writefd(fd, "[DEADLY][");
      die=1;
      break;
    case L_ERROR:
      __writefd(fd, "[ERROR][");
      break;
    case L_WARNING:
      __writefd(fd, "[WARNING][");
      break;
    case L_INFO:
      __writefd(fd, "[INFO][");
      break;
    case L_VERBOSE:
      __writefd(fd, "[VERBOSE][");
      break;
    case L_DEBUG:
      __writefd(fd, "[DEBUG][");
      break;
    default:
      __writefd(fd, "[UNKNOWN][");
      break;
  }
	
  __writefd(fd, facility);
  __writefd(fd, "] ");
  return die;
}

int logmsg_fd(int fd, int level, char *facility, char *msg, ...) {
  va_list ap;
  char *tmp;
  int die=0;

  if (level > current_loglevel) return 0;

  die=logheader(fd, level, facility);

  logwrite(fd, msg);
  va_start(ap, msg);
  while ((tmp = va_arg(ap, char*)))
    logwrite(fd, tmp);
  va_end(ap);
  __writefd(fd, "\n");

  if (die) exit(-1);
  return 0;
}


int logmsg(int level, char *facility, char *msg, ...) {
  va_list ap;
  char *tmp;
  int die=0;

  if (level > current_loglevel) return 0;

  die=logheader(1, level, facility);

  logwrite(1, msg);
  va_start(ap, msg);
  while ((tmp = va_arg(ap, char*)))
    logwrite(1, tmp);
  va_end(ap);
  __write1("\n");

  if (die) exit(-1);
  return 0;
}


