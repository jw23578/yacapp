#ifndef OPENSSLWRAPPER_H
#define OPENSSLWRAPPER_H

#include <openssl/bio.h> /* BasicInput/Output streams */
#include <openssl/err.h> /* errors */
#include <openssl/ssl.h> /* core library */
#include <string>
#include <vector>

class OpenSSLWrapper
{
    EVP_PKEY_CTX *keyCtx;
    EVP_PKEY *keyPair;

    void logErrors(const std::string &file,
                   int line);

public:
    OpenSSLWrapper();

    void createKeyPair();

    std::string getPrivateKey();
    std::string getPublicKey();

    void loadPublicKey(std::string publicKey);
    void loadPrivateKey(std::string privateKey);

    void encrypt(const std::string &message,
                 std::vector<unsigned char> &encryptedMessage,
                 std::vector<unsigned char> &encryptedKey,
                 std::vector<unsigned char> &initialiationVector);
    void decrypt(const std::vector<unsigned char> &encrypted,
                 std::string &decryptedMessage,
                 std::vector<unsigned char> &encryptedKey,
                 std::vector<unsigned char> &initialiationVector);

    std::vector<unsigned char>  md5(const std::vector<unsigned char> &data) const;
};

#endif // OPENSSLWRAPPER_H
