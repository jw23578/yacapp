#ifndef OPENSSLWRAPPER_H
#define OPENSSLWRAPPER_H


#include <openssl/bio.h> /* BasicInput/Output streams */
#include <openssl/err.h> /* errors */
#include <openssl/ssl.h> /* core library */

class OpenSSLWrapper
{
    EVP_PKEY_CTX *keyCtx;
public:
    OpenSSLWrapper();
};

#endif // OPENSSLWRAPPER_H
