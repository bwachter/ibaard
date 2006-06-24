#ifndef _IBAARD_EDIT_H
#define _IBAARD_EDIT_H
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#ifdef __WIN32__
#include <windows.h>
#include <winbase.h>
#else
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#endif

#include "ibaard_types.h"

int edit(char *filename);

#endif
