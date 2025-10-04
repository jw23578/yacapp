#include "messageobject.h"
#include "opensslwrapper.h"
#include "logger.h"
#include "extstring.h"

MessageObject::MessageObject(const QString &id,
                             const QString &senderId,
                             const QString &receiverId,
                             const QDateTime &sent,
                             const QDateTime &received,
                             const QDateTime &deleted_datetime,
                             const QString &content,
                             const bool read,
                             const QString &privateKey)
    : DataObjectInterface{0}
{
    setId(id);
    setSenderId(senderId);
    setReceiverId(receiverId);
    setSent(sent);
    setReceived(received);
    setDeleted_datetime(deleted_datetime);
    if (content.size() && content[0] != '{' && privateKey.size())
    {
        decrypt(content, privateKey);
    }
    else
    {
        setContent(content);
    }
    setRead(read);
}

QString MessageObject::vecToBase64(const std::vector<unsigned char> &vec) const
{
    return QByteArray(reinterpret_cast<const char*>(vec.data()), vec.size()).toBase64();
}

void MessageObject::base64ToVec(const QByteArray &base64, std::vector<unsigned char> &vec)
{
    QByteArray data(QByteArray::fromBase64(base64));
    vec.insert(vec.end(), data.data(), data.data() + data.size());
}

QString MessageObject::encryptToBase64(QString const &receiverPublicKey)
{
    OpenSSLWrapper sslWrapper;
    sslWrapper.loadPublicKey(receiverPublicKey.toStdString());
    std::vector<unsigned char> encryptedMessage;
    std::vector<unsigned char> encryptedKey;
    std::vector<unsigned char> initializationVector;
    sslWrapper.encrypt(content().toStdString(), encryptedMessage, encryptedKey, initializationVector);
    ONLY_DESKTOP_LOG(QString("message: ") + content());
    ONLY_DESKTOP_LOG(QString("encryptedMessageSize: ") + QString::number(encryptedMessage.size()) + " md5: " + ExtString::toHexString(sslWrapper.md5(encryptedMessage)).c_str());
    ONLY_DESKTOP_LOG(QString("encryptedKeySize: ") + QString::number(encryptedKey.size()) + " md5: " + ExtString::toHexString(sslWrapper.md5(encryptedKey)).c_str());
    ONLY_DESKTOP_LOG(QString("initializationVectorSize: ") + QString::number(initializationVector.size()) + " md5: " + ExtString::toHexString(sslWrapper.md5(initializationVector)).c_str());
    return vecToBase64(encryptedMessage) + "-" + vecToBase64(initializationVector) + "-" + vecToBase64(encryptedKey);
    //    return QByteArray(content().toUtf8()).toBase64();
}

void MessageObject::decrypt(const QString &data, const QString &privateKey)
{
    QStringList parts(data.split("-"));
    if (parts.size() != 3)
    {
        return;
    }
    OpenSSLWrapper sslWrapper;
    sslWrapper.loadPrivateKey(privateKey.toStdString());
    std::vector<unsigned char> encryptedMessage;
    base64ToVec(parts[0].toLatin1(), encryptedMessage);
    std::vector<unsigned char> initializationVector;
    base64ToVec(parts[1].toLatin1(), initializationVector);
    std::vector<unsigned char> encryptedKey;
    base64ToVec(parts[2].toLatin1(), encryptedKey);
    ONLY_DESKTOP_LOG(QString("encryptedMessageSize: ") + QString::number(encryptedMessage.size()) + " md5: " + ExtString::toHexString(sslWrapper.md5(encryptedMessage)).c_str());
    ONLY_DESKTOP_LOG(QString("encryptedKeySize: ") + QString::number(encryptedKey.size()) + " md5: " + ExtString::toHexString(sslWrapper.md5(encryptedKey)).c_str());
    ONLY_DESKTOP_LOG(QString("initializationVectorSize: ") + QString::number(initializationVector.size()) + " md5: " + ExtString::toHexString(sslWrapper.md5(initializationVector)).c_str());
    std::string decryptedMessage;
    sslWrapper.decrypt(encryptedMessage, decryptedMessage, encryptedKey, initializationVector);
    setContent(decryptedMessage.c_str());
}

void MessageObject::assign(const MessageObject &other)
{
    setId(other.id());
    setSenderId(other.senderId());
    setReceiverId(other.receiverId());
    setSent(other.sent());
    setReceived(other.received());
    setContent(other.content());
    setRead(other.read());
}
