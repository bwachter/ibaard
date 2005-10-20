#include <stdarg.h>

#include "ibaard.h"
#include "ibaard_log.h"

int dmsg(char *msg, ...){
  //if (!apkg.debug) return 0;
  va_list arguments;

  va_start(arguments, msg);
  while(msg != NULL) {
    __write2(msg);
    msg=va_arg(arguments, char*);
  }
  va_end(arguments);
  return 0;
}
