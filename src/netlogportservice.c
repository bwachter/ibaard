#include "ibaard_network.h"
#include "ibaard_cat.h"
#include "ibaard.h"

void netlogportservice(const struct sockaddr *sa, socklen_t salen, char *msg){
	char host[NI_MAXHOST];
	char serv[NI_MAXSERV];
	char *tmpstring;

	netnameinfo(sa, salen, host, sizeof(host),serv, sizeof(serv),
							NI_NUMERICHOST | NI_NUMERICSERV);
	cat(&tmpstring, msg, host, ":", serv, "\n", NULL);
	__write2(tmpstring);
	free(tmpstring);
}
