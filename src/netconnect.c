#include "ibaard_network.h"
#include "ibaard_log.h"
#include "logtypes.h"

// opens a network connection and returns a socket descriptor
int netconnect(char *hostname, char *servicename){
	struct addrinfo *res;
	struct addrinfo hints;
	int sd, err;
#ifdef __WIN32__
	WSADATA wsaData;
#endif

	memset(&hints, 0, sizeof hints);
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

#ifdef __WIN32__
	WSAStartup( 0x0202, &wsaData );
#endif
	if ((err=netaddrinfo(hostname, servicename, &hints, &res))){
		logmsg(L_ERROR, F_NET, "unable to resolve host", NULL);
		return -1;
	} else {
		while (res){
			if ((sd=netsocket(&*res)) > 0)
				return sd;
			if(res->ai_next==NULL)
				logmsg(L_INFO, F_NET, "res->ai_next is NULL", NULL);
			else 
				logmsg(L_INFO, F_NET, "trying next element", NULL);
			res=res->ai_next;
		}
	}
	return -1;
}
