#include <string.h>
#include <stdlib.h>
#include "ibaard_network.h"
#include "ibaard_log.h"
#include "ibaard_fs.h"
#include "ibaard_cat.h"
#include "logtypes.h"

#if (defined HAVE_SSL) && (!defined HAVE_MATRIXSSL)
int netsslcacert(){
	char *cacert_path = NULL;
#ifdef __WIN32__
	if (getenv("USERPROFILE")!=NULL) {
		if (cat(&cacert_path, getenv("USERPROFILE"), "/.aardmail/cacerts.pem", NULL)) 
#else
	if (getenv("HOME")!=NULL) {
		if (cat(&cacert_path, getenv("HOME"), "/.aardmail/cacerts.pem", NULL)) 
#endif
			return -1;
		else
			strncpy(am_ssl_servercerts, cacert_path, 1024);
	}
	free(cacert_path);
	return 0;
}
#endif
