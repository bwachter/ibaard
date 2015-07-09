#ifndef _IBAARD_STRIP_H
#define _IBAARD_STRIP_H

/** @file
 * Functions to strip some characters from strings
 */

#if (defined _WIN32) || (defined _BROKEN_IO)
#include <stdio.h>
#endif

/** Strip \\n from the end of a string
 *
 * @param buf the string to strip
 * @return a pointer to buf
 */
char *stripn(char *buf);

/** Strip \\r\\n from the end of a string
 *
 * @param buf the string to strip
 * @return a pointer to buf
 */
char *striprn(char *buf);

/** Strip 0x01 from the end of a string
 *
 * @param buf the string to strip
 * @return a pointer to buf
 */
char *stripa(char *buf);

#endif
