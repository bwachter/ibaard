#include "ibaard_network.h"

#if (defined(__WIN32__)) || (defined(_BROKEN_IO))
static struct addrinfo * dup_addrinfo (struct addrinfo *info, void *addr, size_t addrlen) {
  struct addrinfo *ret;

  ret = malloc (sizeof (struct addrinfo));
  if (ret == NULL)
    return NULL;
  memcpy (ret, info, sizeof (struct addrinfo));
  ret->ai_addr = malloc (addrlen);
  if (ret->ai_addr == NULL)
  {
    free (ret);
    return NULL;
  }
  memcpy (ret->ai_addr, addr, addrlen);
  ret->ai_addrlen = addrlen;
  return ret;
}
#endif

void netfreeaddrinfo (struct addrinfo *ai) {
#if (defined(__WIN32__)) || (defined(_BROKEN_IO))
  struct addrinfo *next;

  while (ai != NULL)
  {
    next = ai->ai_next;
    if (ai->ai_canonname != NULL)
      free (ai->ai_canonname);
    if (ai->ai_addr != NULL)
      free (ai->ai_addr);
    free (ai);
    ai = next;
  }
#else
  freeaddrinfo(ai);
#endif
}

int netaddrinfo(const char *node, const char *service, 
                const struct addrinfo *hints, struct addrinfo **res){
  int err;
#ifdef __WIN32__
  HINSTANCE _hInstance = LoadLibrary( "ws2_32" );
  int (WSAAPI *pfn_getaddrinfo) (const char*, const char*, const struct addrinfo*, struct addrinfo **);

  pfn_getaddrinfo =	GetProcAddress( _hInstance, "getaddrinfo" );

  if (pfn_getaddrinfo){
    return (err=pfn_getaddrinfo(node, service, hints, res));
  } else {
#endif
#if (defined(__WIN32__)) || (defined(_BROKEN_IO))
    struct hostent *hp;
    struct servent *servent;
    const char *socktype;
    int port;
    struct addrinfo hint, result;
    struct addrinfo *ai, *sai, *eai;
    char **addrs;
    err=0;

    memset (&result, 0, sizeof result);

    /* default for hints */
    if (hints == NULL)
    {
      memset (&hint, 0, sizeof hint);
      hint.ai_family = PF_UNSPEC;
      hints = &hint;
    }

    /* servname must not be NULL in this implementation */
    if (service == NULL)
      return EAI_NONAME;

    /* check for tcp or udp sockets only */
    if (hints->ai_socktype == SOCK_STREAM)
      socktype = "tcp";
    else if (hints->ai_socktype == SOCK_DGRAM)
      socktype = "udp";
    else
      return EAI_SERVICE;
    result.ai_socktype = hints->ai_socktype;

    /* Note: maintain port in host byte order to make debugging easier */
    if (isdigit (*service))
      port = strtol (service, NULL, 10);
    else if ((servent = getservbyname (service, socktype)) != NULL)
      port = ntohs (servent->s_port);
    else
      return EAI_NONAME;

    /* if nodename == NULL refer to the local host for a client or any
       for a server */
    if (node == NULL)
    {
      struct sockaddr_in sin;

      /* check protocol family is PF_UNSPEC or PF_INET - could try harder
         for IPv6 but that's more code than I'm prepared to write */
      if (hints->ai_family == PF_UNSPEC || hints->ai_family == PF_INET)
	result.ai_family = AF_INET;
      else
	return EAI_FAMILY;

      sin.sin_family = result.ai_family;
      sin.sin_port = htons (port);
      if (hints->ai_flags & AI_PASSIVE)
        sin.sin_addr.s_addr = htonl (INADDR_ANY);
      else
        sin.sin_addr.s_addr = htonl (INADDR_LOOPBACK);
      /* Duplicate result and addr and return */
      *res = dup_addrinfo (&result, &sin, sizeof sin);
      return (*res == NULL) ? EAI_MEMORY : 0;
    }

    /* If AI_NUMERIC is specified, use inet_addr to translate numbers and
       dots notation. */
    if (hints->ai_flags & AI_NUMERICHOST)
    {
      struct sockaddr_in sin;

      /* check protocol family is PF_UNSPEC or PF_INET */
      if (hints->ai_family == PF_UNSPEC || hints->ai_family == PF_INET)
	result.ai_family = AF_INET;
      else
	return EAI_FAMILY;

      sin.sin_family = result.ai_family;
      sin.sin_port = htons (port);
      sin.sin_addr.s_addr = inet_addr (node);
      /* Duplicate result and addr and return */
      *res = dup_addrinfo (&result, &sin, sizeof sin);
      return (*res == NULL) ? EAI_MEMORY : 0;
    }

    hp = gethostbyname (node);
    // fixme, translate error codes
    if (hp == NULL) return h_errno;

    /* Check that the address family is acceptable.
     */
    switch (hp->h_addrtype)
    {
      case AF_INET:
        if (!(hints->ai_family == PF_UNSPEC || hints->ai_family == PF_INET))
          return EAI_FAMILY;
        break;
      case AF_INET6:
        if (!(hints->ai_family == PF_UNSPEC || hints->ai_family == PF_INET6))
          return EAI_FAMILY;
        break;
      default:
        return EAI_FAMILY;
    }

    /* For each element pointed to by hp, create an element in the
       result linked list. */
    sai = eai = NULL;
    for (addrs = hp->h_addr_list; *addrs != NULL; addrs++)
    {
      struct sockaddr sa;
      size_t addrlen;

      if (hp->h_length < 1)
        continue;
      sa.sa_family = hp->h_addrtype;
      switch (hp->h_addrtype)
      {
        case AF_INET:
	  ((struct sockaddr_in *) &sa)->sin_port = htons (port);
	  memcpy (&((struct sockaddr_in *) &sa)->sin_addr,
	          *addrs, hp->h_length);
          addrlen = sizeof (struct sockaddr_in);
          break;
        case AF_INET6:
# if SIN6_LEN
	  ((struct sockaddr_in6 *) &sa)->sin6_len = hp->h_length;
# endif
	  ((struct sockaddr_in6 *) &sa)->sin6_port = htons (port);
	  memcpy (&((struct sockaddr_in6 *) &sa)->sin6_addr,
	  	  *addrs, hp->h_length);
          addrlen = sizeof (struct sockaddr_in6);
          break;
        default:
          continue;
      }

      result.ai_family = hp->h_addrtype;
      ai = dup_addrinfo (&result, &sa, addrlen);
      if (ai == NULL)
      {
        netfreeaddrinfo (sai);
        return EAI_MEMORY;
      }
      if (sai == NULL)
	sai = ai;
      else
	eai->ai_next = ai;
      eai = ai;
    }

    if (sai == NULL)
    {
      return EAI_NODATA;
    }
  
    if (hints->ai_flags & AI_CANONNAME) 
    {
      sai->ai_canonname = malloc (strlen (hp->h_name) + 1);
      if (sai->ai_canonname == NULL)
      {
        netfreeaddrinfo (sai);
        return EAI_MEMORY;
      }
      strcpy (sai->ai_canonname, hp->h_name);
    }

    *res = sai;
    return 0;
#ifdef __WIN32__
  }
#endif
#else
  (void)err;
  return (getaddrinfo(node, service, hints, res));
#endif
}
