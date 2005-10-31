#include <stdarg.h>

#include "ibaard.h"
#include "ibaard_log.h"

int lmsg(char *msg, ...){
  va_list arguments;

  va_start(arguments, msg);
  while(msg != NULL) {
    __write1(msg);
    msg=va_arg(arguments, char*);
  }
  va_end(arguments);
  return 0;
}

