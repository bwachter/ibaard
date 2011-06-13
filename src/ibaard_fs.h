#ifndef _IBAARD_FS_H
#define _IBAARD_FS_H

/** @file
 * Functions to manipulate filesystem objects
 */

#if (defined __WIN32__) || (defined _BROKEN_IO)
#include <stdio.h>
#endif

#include <fcntl.h>
#include <sys/stat.h>

/** Test if file exists
 *
 * @todo complete documentation
 */
int tf(char *name);

/** Write string to file
 *
 * @todo complete documentation
 */
int wf(char *name, char *content);

/** Test if directory exists
 *
 * @todo complete documentation
 */
int td(char *name);

/** Create a directory
 *
 * @todo complete documentation
 */
int md(char* name);
#ifndef __WIN32MSC
/** Create a directory and set mode
 *
 * @todo complete documentation
 */

int md2(char* name, mode_t mode);
#endif

/** Create a FIFO
 *
 * @todo complete documentation
 */
int mf(char* name);

/** Create a FIFO and set mode
 *
 * @todo complete documentation
 */
int mf2(char* name, mode_t mode);

/** Read contents of file
 *
 * @todo complete documentation
 */
int rdf(char *name, char **buf);

/** Create a new, empty file
 *
 * @todo complete documentation
 */
int touch(char *name);

/** Read contents of file
 *
 * @todo complete documentation
 */
int openreadclose(char *fn, char **buf, unsigned long *len);

/** Write to FILE or file descriptor, depending on platform
 *
 * @todo complete documentation
 */
#if (defined __WIN32__) || (defined _BROKEN_IO)
int filewrite(FILE *fd, char *buf, int len);
#else
int filewrite(int fd, char *buf, int len);
#endif

/** Return current directory with unlimited length
 *
 * @note ripped from busybox
 * @todo complete documentation
 */
char *xgetcwd (char *cwd);

#endif
