/**
 * @file netsslcacert.c
 * @author Bernd Wachter <bwachter@lart.info>
 * @date 2005-2011
 */

#include <string.h>
#include <stdlib.h>
#include "ibaard_network.h"
#include "ibaard_log.h"
#include "ibaard_fs.h"
#include "ibaard_cat.h"
#include "logtypes.h"

#if (defined HAVE_SSL) && (!defined HAVE_MATRIXSSL)
int netsslcacert(char *subdir){
  char *cacert_path = NULL;
  char *cacert = NULL;

  if (subdir != NULL) {
    if (cat(&cacert, subdir, "/cacerts.pem", NULL)) return -1;
  } else if (cat(&cacert, ".libaard/cacerts.pem", NULL)) return -1;

#ifdef _WIN32
  if (getenv("USERPROFILE")!=NULL) {
    if (cat(&cacert_path, getenv("USERPROFILE"), "/", cacert, NULL)) {
#else
      if (getenv("HOME")!=NULL) {
        if (cat(&cacert_path, getenv("HOME"), "/", cacert, NULL)) {
#endif
          free(cacert);
          return -1;
        } else
          strncpy(am_ssl_servercerts, cacert_path, 1024);
      }
      logmsg(L_INFO, F_SSL, cacert_path, NULL);
      free(cacert);
      free(cacert_path);
      return 0;
    }
#endif
