#include "ibaard_network.h"
#include "ibaard_log.h"
#include "logtypes.h"

int netwriteline(int sd, char *buf){
	int i;

	logmsg(L_VERBOSE, F_NET, "> ", buf, NULL);

#if (defined HAVE_SSL) || (defined HAVE_MATRIXSSL)
	if (am_sslconf & AM_SSL_USETLS){
		i=netsslwrite(ssl, buf, strlen(buf));
	} else
#endif
		i=send(sd, buf, strlen(buf), 0);
	return i;
}
