#include "yacservernetwork.h"
#include <QFileInfo>
#include <QDir>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

YACServerNetwork::YACServerNetwork(QNetworkAccessManager &manager
                       , const Constants &constants):
    YACServerBaseNetwork(manager, constants)
{
}




void YACServerNetwork::yacappServerGetAllAPPs(CallbackFunction successCallback,
                                        CallbackFunction errorCallback)
{
    auto replyHandler([](QNetworkReply *finishedReply, SRunningRequest &rr)
    {
        QByteArray all(finishedReply->readAll());
        rr.successCallback(all);
    });
    const QUrlQuery query;
    yacappServerGet("/getAllAPPs",
                    query,
                    replyHandler,
                    successCallback,
                    errorCallback);
}

void YACServerNetwork::yacappServerGetAPP(const QString &app_id,
                                    const int current_installed_version,
                                    CallbackFunction successCallback,
                                    CallbackFunction errorCallback)
{
    auto replyHandler([this](QNetworkReply *finishedReply, SRunningRequest &rr)
    {
        QByteArray all(finishedReply->readAll());
        QJsonDocument replyDoc(QJsonDocument::fromJson(all));
        QJsonObject object(replyDoc.object());
        QString message(object["message"].toString());
        if (message == "app version is up to date")
        {
            rr.successCallback(message);
            return;
        }
        if (message == "app found")
        {
            QString json_yacapp(object["json_yacapp"].toString());
            QString app_id(object["app_id"].toString());

            QByteArray yacpck_base64(object["yacpck_base64"].toString().toLatin1());
            QByteArray yacpck(QByteArray::fromBase64(yacpck_base64));
            QFile file(constants.getYacAppConfigPath() + app_id + ".yacapp");
            file.open(QIODevice::WriteOnly);
            file.write(json_yacapp.toLatin1());
            file.close();

            QByteArray uncompressedData(qUncompress(yacpck));
            int pos(0);
            while (pos < uncompressedData.size())
            {
                int nextPos(uncompressedData.indexOf('\0', pos));
                QString filename(uncompressedData.mid(pos, nextPos - pos));
                pos = nextPos + 1;
                nextPos = uncompressedData.indexOf('\0', pos);
                if (nextPos == -1)
                {
                    nextPos = uncompressedData.size();
                }
                QByteArray data(uncompressedData.mid(pos, nextPos - pos));

                QFile file(constants.getYacAppConfigPath() + filename);
                file.open(QIODevice::WriteOnly);
                file.write(data);
                file.close();
                pos = nextPos + 1;
            }
            rr.successCallback(message);
        }
        else
        {
            rr.errorCallback(message);
        }
    });
    QUrlQuery query;
    query.addQueryItem("app_id", app_id);
    query.addQueryItem("current_installed_version", QString::number(current_installed_version));
    yacappServerGet("/getAPP",
                    query,
                    replyHandler,
                    successCallback,
                    errorCallback);
}

void YACServerNetwork::yacappServerAppUserRegister(const QString &loginEMail,
                                                   const QString &password,
                                                   const QString &appId,
                                                   CallbackFunction successCallback,
                                                   CallbackFunction errorCallback)
{
    auto replyHandler([this](QNetworkReply *finishedReply, SRunningRequest &rr)
    {
        QByteArray all(finishedReply->readAll());
        QJsonDocument replyDoc(QJsonDocument::fromJson(all));
        QJsonObject object(replyDoc.object());
        QString message(object["message"].toString());
        int t = 0;
    });

    QJsonObject obj;
    obj["loginEMail"] = loginEMail;
    obj["password"] = password;
    obj["appId"] = appId;
    yacappServerPost("/registerAppUser",
                     obj,
                     replyHandler,
                     successCallback,
                     errorCallback);
}

void YACServerNetwork::yacappServerAppUserVerify(const QString &loginEMail,
                                                 const QString &verifyToken,
                                                 CallbackFunction successCallback,
                                                 CallbackFunction errorCallback)
{
    auto replyHandler([this](QNetworkReply *finishedReply, SRunningRequest &rr)
    {
        QByteArray all(finishedReply->readAll());
        QJsonDocument replyDoc(QJsonDocument::fromJson(all));
        QJsonObject object(replyDoc.object());
        QString message(object["message"].toString());
        rr.successCallback(message);
    });

    QJsonObject obj;
    obj["loginEMail"] = loginEMail;
    obj["verifyToken"] = verifyToken;
    yacappServerPost("/verifyAppUser",
                     obj,
                     replyHandler,
                     successCallback,
                     errorCallback);
}
