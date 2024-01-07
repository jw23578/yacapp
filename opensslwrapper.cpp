#include "opensslwrapper.h"
#include <openssl/evp.h>
#include <openssl/md5.h>
#include "logger.h"

void OpenSSLWrapper::logErrors(const std::string &file, int line)
{
    unsigned long e(ERR_get_error());
    while (e)
    {
        char message[1024];
        ERR_error_string_n(e, message, 1024);
        Logger::gi().log(file.c_str(), line, {Logger::Fatal}, message);
    }
}

OpenSSLWrapper::OpenSSLWrapper():
    keyCtx(EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL)),
    keyPair(NULL)
{
}

void OpenSSLWrapper::createKeyPair()
{
    EVP_PKEY_keygen_init(keyCtx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(keyCtx, 4096); // RSA 4096
    // generate key
    EVP_PKEY_keygen(keyCtx, &keyPair);
    // free up key context
    EVP_PKEY_CTX_free(keyCtx);
}

std::string OpenSSLWrapper::getPrivateKey()
{
    // extract private key as string
    // create a place to dump the IO, in this case in memory
    BIO *privateBIO = BIO_new(BIO_s_mem());
    // dump key to IO
    PEM_write_bio_PrivateKey(privateBIO, keyPair, NULL, NULL, 0, 0, NULL);
    // get buffer length
    int privateKeyLen = BIO_pending(privateBIO);
    // create char reference of private key length
    char *privateKeyChar = (char *) malloc(privateKeyLen);
    // read the key from the buffer and put it in the char reference
    BIO_read(privateBIO, privateKeyChar, privateKeyLen);
    // at this point we can save the private key somewhere
    return std::string(privateKeyChar, privateKeyLen);
}

std::string OpenSSLWrapper::getPublicKey()
{
    // extract public key as string
    // create a place to dump the IO, in this case in memory
    BIO *publicBIO = BIO_new(BIO_s_mem());
    // dump key to IO
    PEM_write_bio_PUBKEY(publicBIO, keyPair);
    // get buffer length
    int publicKeyLen = BIO_pending(publicBIO);
    // create char reference of public key length
    char *publicKeyChar = (char *) malloc(publicKeyLen);
    // read the key from the buffer and put it in the char reference
    BIO_read(publicBIO, publicKeyChar, publicKeyLen);
    // at this point we can save the public somewhere
    return std::string(publicKeyChar, publicKeyLen);
}

void OpenSSLWrapper::loadPublicKey( std::string publicKey)
{
    // write char array to BIO
    if (!publicKey.size())
    {
        FATAL_LOG("no publicKey given");
    }
    if (publicKey.find("END PUBLIC KEY") == std::string::npos)
    {
        FATAL_LOG(QString("publicKey probaby in wrong format: ") + publicKey.c_str());
    }
    BIO *rsaPublicBIO = BIO_new_mem_buf(publicKey.data(), -1);
    logErrors(__FILE__, __LINE__);
    PEM_read_bio_PUBKEY(rsaPublicBIO, &keyPair, NULL, NULL);
    logErrors(__FILE__, __LINE__);
    BIO_free(rsaPublicBIO);
}

void OpenSSLWrapper::loadPrivateKey(std::string privateKey)
{
    // write char array to BIO
    BIO *rsaPrivateBIO = BIO_new_mem_buf(privateKey.data(), -1);
    // create a RSA object from private key char array
    PEM_read_bio_PrivateKey(rsaPrivateBIO, &keyPair, NULL, NULL);

    BIO_free(rsaPrivateBIO);
}

void OpenSSLWrapper::encrypt(const std::string &message,
                             std::vector<unsigned char> &encryptedMessage,
                             std::vector<unsigned char> &encryptedKey,
                             std::vector<unsigned char> &initialiationVector)
{
    EVP_CIPHER_CTX *rsaEncryptCtx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(rsaEncryptCtx);


    // variables for where the encrypted secret, length, and IV reside
    unsigned char *ek((unsigned char *)malloc(EVP_PKEY_size(keyPair)));
    int encryptedKeyLen = 0;
    unsigned char *iv((unsigned char *)malloc(EVP_MAX_IV_LENGTH));
    // generate AES secret, and encrypt it with public key
    EVP_SealInit(rsaEncryptCtx, EVP_aes_256_cbc(), &ek, &encryptedKeyLen, iv, &keyPair, 1);
    logErrors(__FILE__, __LINE__);
    // encrypt a message with AES secret
    const unsigned char* messageChar = (const unsigned char*) message.c_str();
    // length of message
    int messageLen = message.size() + 1;
    // create char reference for where the encrypted message will reside
    unsigned char *tempEncryptedMessage = (unsigned char *) malloc(messageLen + EVP_MAX_IV_LENGTH);
    // the length of the encrypted message
    int encryptedMessageLen = 0;
    int encryptedBlockLen = 0;
    // encrypt message with AES secret
    EVP_SealUpdate(rsaEncryptCtx, tempEncryptedMessage, &encryptedBlockLen, messageChar, messageLen);
    encryptedMessageLen = encryptedBlockLen;
    // finalize by encrypting the padding
    EVP_SealFinal(rsaEncryptCtx, tempEncryptedMessage + encryptedBlockLen, &encryptedBlockLen);
    encryptedMessageLen += encryptedBlockLen;
    encryptedMessage.insert(encryptedMessage.end(), tempEncryptedMessage, tempEncryptedMessage + encryptedMessageLen);
    encryptedKey.insert(encryptedKey.end(), ek, ek + encryptedKeyLen);
    initialiationVector.insert(initialiationVector.begin(), iv, iv + EVP_MAX_IV_LENGTH);
    free(iv);
    free(ek);
}

void OpenSSLWrapper::decrypt(const std::vector<unsigned char> &encrypted,
                             std::string &decryptedMessage,
                             std::vector<unsigned char> &encryptedKey,
                             std::vector<unsigned char> &initialiationVector)
{
    EVP_CIPHER_CTX *rsaDecryptCtx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(rsaDecryptCtx);
    // decrypt EK with private key, and get AES secretp

    EVP_OpenInit(rsaDecryptCtx, EVP_aes_256_cbc(), encryptedKey.data(), encryptedKey.size(), initialiationVector.data(), keyPair);
//    EVP_OpenInit(rsaDecryptCtx, EVP_aes_256_cbc(), ek, encryptedKeyLen, iv, keyPair);

    // variable for where the decrypted message with be outputed to
    unsigned char *tempDecryptedMessage = (unsigned char *) malloc(encrypted.size() + EVP_MAX_IV_LENGTH);
    // the length of the encrypted message
    int decryptedMessageLen = 0;
    int decryptedBlockLen = 0;
    // decrypt message with AES secret
    EVP_OpenUpdate(rsaDecryptCtx, tempDecryptedMessage, &decryptedBlockLen, encrypted.data(), encrypted.size());
    decryptedMessageLen = decryptedBlockLen;
    // finalize by decrypting padding
    EVP_OpenFinal(rsaDecryptCtx, tempDecryptedMessage + decryptedBlockLen, &decryptedBlockLen);
    decryptedMessageLen += decryptedBlockLen;

    decryptedMessage.insert(decryptedMessage.end(), tempDecryptedMessage, tempDecryptedMessage + decryptedMessageLen - 1);
}

std::vector<unsigned char> OpenSSLWrapper::md5(const std::vector<unsigned char> &data) const
{
    std::vector<unsigned char> result(MD5_DIGEST_LENGTH);
    MD5(data.data(), data.size(), result.data());
    return result;
}
