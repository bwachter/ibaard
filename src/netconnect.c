#include "ibaard_network.h"
#include "ibaard_log.h"
#include "logtypes.h"

// opens a network connection and returns a socket descriptor
int netconnect(char *hostname, char *servicename){
  return netconnect2(hostname, servicename, (char*)NULL);
}

int netconnect2(char *hostname, char *servicename, char *bindaddress){
  struct addrinfo *res, *bindres;
  struct addrinfo hints;
  int sd, err;
#ifdef __WIN32__
  WSADATA wsaData;
#endif

  if (bindaddress==NULL)
    logmsg(L_ERROR, F_NET, "not trying to bind to anything", NULL);
  else
    logmsg(L_ERROR, F_NET, "will try binding", NULL);

  memset(&hints, 0, sizeof hints);
  hints.ai_flags = AI_PASSIVE;
  hints.ai_socktype = SOCK_STREAM;

#ifdef __WIN32__
  WSAStartup( 0x0202, &wsaData );
#endif
  if (bindaddress!=NULL){
    if ((err=netaddrinfo(bindaddress, NULL, &hints, &bindres))){
      logmsg(L_ERROR, F_NET, "unable to resolve local bind address", NULL);
      return -1;
    }
  }

  if ((err=netaddrinfo(hostname, servicename, &hints, &res))){
    logmsg(L_ERROR, F_NET, "unable to resolve host", NULL);
    netfreeaddrinfo(bindres);
    return -1;
  } else {
    struct addrinfo *r, *b;
    
    r=res;
    while (r){
      if (bindaddress!=NULL){
        // so far we try to match only the first record of bindres
        // TODO: check all entries
        b=bindres;
        if (b->ai_family==r->ai_family)
          if ((sd=netsocket2(&*r, &*b)) > 0){
            netfreeaddrinfo(bindres);
            netfreeaddrinfo(res);
            return sd;
          }
      } else {
        if ((sd=netsocket(&*r)) > 0){
          if (bindaddress!=NULL) netfreeaddrinfo(bindres);
          netfreeaddrinfo(res);
          return sd;
        }
      }
      if(r->ai_next==NULL)
        logmsg(L_INFO, F_NET, "r->ai_next is NULL", NULL);
      else 
        logmsg(L_INFO, F_NET, "trying next element", NULL);
      r=r->ai_next;
    }
  }
  if (bindaddress!=NULL) netfreeaddrinfo(bindres);
  netfreeaddrinfo(res);
  return -1;
}
