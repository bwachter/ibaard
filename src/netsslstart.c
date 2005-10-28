#include "ibaard_network.h"
#include "ibaard_log.h"
#include "logtypes.h"

#if (defined HAVE_SSL) && (!defined HAVE_MATRIXSSL)

// in case we found a certificate we'll set it before negotiating any ssl
// stuff. If the other site requests it on handshake SSL will just return 
// it, if the other site requests it during normal operation SSL will give
// us SSL_ERROR_WANT_WRITE or SSL_ERROR_WANT_READ -- which basically means
// that OpenSSL takes care about sending the cert _now_ and wants us to
// repeat the last operation. In case we did _not_ set a certificate 
// OpenSSL will call the client_cert_cb()-hook if one is requested. Since 
// we were unable to load a certifikate on startup we guess we still would
// not get it, and use this method just as a simple way to report a 
// certificate problem 
static int provide_client_cert(SSL *ssl, X509 **cert, EVP_PKEY **pkey)
{
	(void)ssl;
	(void)cert;
	(void)pkey;
  logmsg(L_ERROR, F_SSL, "The server requested a client certificate, but you did not specify one\n", NULL);
  return 0;
}

int netsslstart(int sd){
	int err;
	SSL_CTX *ctx;
	//	X509 *server_cert;

	// we're not meant to use ssl, return
	if (!(am_sslconf & AM_SSL_USETLS)) return -1;

	SSL_load_error_strings();
	SSLeay_add_ssl_algorithms();
	ctx = SSL_CTX_new(SSLv23_client_method());
	if (!ctx){
		logmsg(L_ERROR, F_SSL, "problem with SSL_CTX_new()", NULL);
		am_sslconf ^= AM_SSL_USETLS; // remove usetls-bits
		return  -1;
	}

	if (strcmp(am_sslkey, "")){
		if ((SSL_CTX_use_certificate_chain_file(ctx, am_sslkey))!=1){
			logmsg(L_ERROR, F_SSL, "problem with SSL certfoo", NULL);
			am_sslconf ^= AM_SSL_USETLS; 
			return  -1;
		} 
		if ((SSL_CTX_use_PrivateKey_file(ctx, am_sslkey, SSL_FILETYPE_PEM))!=1){
			logmsg(L_ERROR, F_SSL, "problem with SSL certfoo", NULL);
			am_sslconf ^= AM_SSL_USETLS; 
			return  -1;
		}
	} else
		SSL_CTX_set_client_cert_cb(ctx, provide_client_cert);

	ssl = SSL_new(ctx);
	if (!ssl) {
		logmsg(L_ERROR, F_SSL, "ssl not working\n", NULL);
		am_sslconf ^= AM_SSL_USETLS;
		return -1;
	}
	SSL_set_fd(ssl, sd);

	if ((err = SSL_connect(ssl)) < 0){
		logmsg(L_ERROR, F_SSL, "error on SSL_connect(): ", strerror(errno), NULL);
		am_sslconf ^= AM_SSL_USETLS;
		return -1;
	} else
		am_sslconf |= AM_SSL_USETLS; // enable usetls
	logmsg(L_INFO, F_SSL, "SSL-connection using ", (SSL_get_cipher(ssl)), NULL);
	return 0;
}
#endif
