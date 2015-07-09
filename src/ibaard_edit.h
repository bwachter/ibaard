#ifndef _IBAARD_EDIT_H
#define _IBAARD_EDIT_H

/** @file
 *
 * Editing things
 */

#include "ibaard_platform.h"

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#ifdef _WIN32
#include <winsock2.h>
// fucking great, msvc will die if we don't put this everywhere
// before including windows.h
#include <windows.h>
#include <winbase.h>
#else
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#endif

#include "ibaard_types.h"

/** Open a file in the default editor
 *
 * edit() will try to open a file in the editor set in the EDITOR environment
 * variable or, if this does not exist, in vi.
 *
 * @param filename the file to open
 * @return -1 on error, or the (old!) pid of the editor on success
 */
int edit(char *filename);

#endif
