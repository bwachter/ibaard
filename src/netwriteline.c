#include "ibaard_network.h"
#define F_NET "net"

int netwriteline(int sd, char *buf){
	int i;

	logmsg(L_INFO, F_NET, "> ", buf, NULL);

#if (defined HAVE_SSL) || (defined HAVE_MATRIXSSL)
	if (am_sslconf & AM_SSL_USETLS){
		i=netsslwrite(ssl, buf, strlen(buf));
	} else
#endif
		i=send(sd, buf, strlen(buf), 0);
	return i;
}
