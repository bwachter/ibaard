#ifndef IBAARD_CAT_H
#define IBAARD_CAT_H

/** @file
 */

/** Concatenate strings into a buffer
 *
 * @note cat() allocates the needed memory by itself, but for obvious
 * reasons does not free it. Remember to free the memory as soon as you
 * don't need it anymore.
 *
 * @param dest the buffer to store the new string
 * @param str the first string
 * @param ... the remaining strings, terminated by NULL
 * @return 0 on success, -1 on error
 */
int cat(char **dest, char *str, ...);

/** Concatenate strings into a buffer with limit
 *
 * @note: You should free the buffer malloc()d by cat after use
 *
 * @param dest the buffer to store the new string
 * @param len the maximum string length
 * @param str the first string
 * @param ... the remaining strings, terminated by NULL
 * @return 0 on success, -1 on error
 */
int catn(char **dest, int len, char *str, ...);

/** Concatenate strings inline
 *
 * @note cati() uses a shared buffer. Don't use cati in functions which
 * expect strings as arguments. Due to the shared memory bad things happen
 * if the function gets called with cati in one argument.
 *
 * @todo Add option to specify different levels of buffers to avoid the above
 * problem
 *
 * @param str the first string
 * @param ... the remaining strings, terminated by NULL
 * @return a pointer to the concatenated string
 */
char *cati(char *str, ...);

/** Free the memory used by cati()
 */
void freecati();
#endif
