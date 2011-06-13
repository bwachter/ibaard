#ifndef _IBAARD_LOG_H
#define _IBAARD_LOG_H

/** @file
 * Log related functions
 */

/** Log levels used in logging functions */
enum LOGLEVELS {
  L_UNSPEC,
  L_DEADLY,
  L_ERROR,
  L_WARNING,
  L_INFO,
  L_VERBOSE,
  L_DEBUG,
};

/** Write a log message to stdout
 *
 * @todo complete documentation
 */
int logmsg(int level, char *facility, char *msg, ...);

/** Write a log message to fd
 *
 * @todo complete documentation
 */
int logmsg_fd(int fd, int level, char *facility, char *msg, ...);

/** Change or get loglevel
 *
 * @param level 0 to check loglevel, any value from LOGLEVELS to set level
 * @return the current loglevel, or the new loglevel set
 */
int loglevel(int level);

/** Write a message to stdout
 *
 * @todo complete documentation
 */
int lmsg(char *msg, ...);

/** Write a message to stderr
 *
 * @todo complete documentation
 */
int emsg(char *msg, ...);

#endif
