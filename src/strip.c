#include <string.h>
#ifndef __WIN32__
#include <strings.h>
#endif
#include "ibaard_strip.h"

/* strip \n at end of buf */
char *stripn(char *buf) {
  if (strncasecmp(buf+strlen(buf)-1, "\n", 1) == 0) {
    buf[strlen(buf)-1]='\0';
  }
  return buf;
}

/* strip \r\n at end of buf */
char *striprn(char *buf) {
  if (strncasecmp(buf+strlen(buf)-2, "\r\n", 2) == 0) {
    buf[strlen(buf)-2]='\0';
  }
  return buf;
}

/* strip 0x01 at end of buf */
char *stripa(char *buf) {
  if (strncasecmp(buf+strlen(buf)-1, "\x01", 1) == 0) {
    buf[strlen(buf)-1]='\0';
  }
  return buf;
}

