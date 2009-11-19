#include "ibaard_network.h"
#include "ibaard_cat.h"
#include "ibaard_log.h"
#include "ibaard.h"
#include "logtypes.h"

void netlogportservice(const struct sockaddr *sa, socklen_t salen, char *msg){
  char host[NI_MAXHOST];
  char serv[NI_MAXSERV];
  char *tmpstring=NULL;

  netnameinfo(sa, salen, host, sizeof(host),serv, sizeof(serv),
              NI_NUMERICHOST | NI_NUMERICSERV);
  cat(&tmpstring, msg, host, ":", serv, NULL);
  logmsg(L_DEBUG, F_NET, tmpstring, NULL);
  free(tmpstring);
}
