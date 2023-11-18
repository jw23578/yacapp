#include "opensslwrapper.h"

OpenSSLWrapper::OpenSSLWrapper():
    keyCtx(EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL))
{
    EVP_PKEY_keygen_init(keyCtx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(keyCtx, 4096); // RSA 4096
}
