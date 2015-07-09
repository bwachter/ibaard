#ifndef _IBAARD_FS_H
#define _IBAARD_FS_H

/** @file
 * Functions to manipulate filesystem objects
 */

#if (defined _WIN32) || (defined _BROKEN_IO)
#include <stdio.h>
#endif

#include <fcntl.h>
#include <sys/stat.h>

/** Test if file exists _and_ can be opened by the user
 *
 * @param name The filename to test
 * @return 0 if the file exists and is readable, negative values (errno) otherwise
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
#ifndef _WIN32
/** Create a directory and set mode
 *
 * @todo complete documentation
 */

int md2(char* name, mode_t mode);
#endif

/** Create a FIFO
 *
 * The mode for the new FIFO will be 666, with the value of umask applied.
 * For a common umask with 0022 this will give you a FIFO with 644.
 *
 * If the FIFO already exists the mode will be set to 0666&~umask
 *
 * @param name The name of the FIFO
 * @return 0 on success, -1 on error
 *         (unable to create, or exists, but not as FIFO)
 */
int mf(char* name);
#ifndef _WIN32
/** Create a FIFO and set mode
 *
 * The function will ignore umask when setting the mode for the new FIFO.
 * If the FIFO already exists only the mode will be adjusted.
 *
 * @param name The name of the FIFO
 * @param mode The mode for the new FIFO
 * @return 0 on success, -1 on error
 *         (unable to create, or exists, but not as FIFO)
 */
int mf2(char* name, mode_t mode);
#endif

#ifndef _WIN32
/** Read contents of file
 *
 * @todo complete documentation
 */
int rdf(char *name, char **buf);
#endif

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
#if (defined _WIN32) || (defined _BROKEN_IO)
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
