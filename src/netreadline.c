#include "ibaard_network.h"
#include "ibaard_log.h"
#include "logtypes.h"

int netreadline(int sd, char *buf){
	int i,cnt;
	char tmpbuf[1];

	buf[0]='\0';
	for (cnt=0; cnt<MAXNETBUF-2; cnt++){
#if (defined HAVE_SSL) || (defined HAVE_MATRIXSSL)
		if (am_sslconf & AM_SSL_USETLS){
			i=netsslread(ssl, tmpbuf, 1);
		} else
#endif
			i=recv(sd, tmpbuf, 1, 0);
		if (i == -1) return -1;
		else{
			if (tmpbuf[0] == '\0'){
				// nullbyte handling will work as long as any functions using
				// our output won't rely on strlen()
				logmsg(L_WARNING, F_NET, "nullbyte detected, mail might be corrupted", NULL);
				logmsg(L_INFO, F_NET, "< ", buf, NULL);
				return cnt;
			} else {
				strncat(buf, tmpbuf, 1);
				if (buf[cnt] == '\n' && buf[cnt-1]=='\r'){
					logmsg(L_INFO, F_NET, "< ", buf, NULL);
					return cnt; 
				}
			}
		}
	}
	logmsg(L_INFO, F_NET, "< ", buf, NULL);
	return MAXNETBUF-3;
}
