#ifndef _IBAARD_STRIP_H
#define _IBAARD_STRIP_H

#if (defined __WIN32__) || (defined _BROKEN_IO)
#include <stdio.h>
#endif

/* strip different chars from string
 * -- strip.c  */
char *stripn(char *buf);
char *striprn(char *buf);
char *stripa(char *buf);

#endif
