/**
 * @file emsg.c
 * @author Bernd Wachter <bwachter@lart.info>
 * @date 2005-2011
 */

#include <stdarg.h>

#include "ibaard.h"
#include "ibaard_log.h"

int emsg(char *msg, ...){
  va_list arguments;

  va_start(arguments, msg);
  while(msg != NULL) {
    __write2(msg);
    msg=va_arg(arguments, char*);
  }
  va_end(arguments);
  return 0;
}
