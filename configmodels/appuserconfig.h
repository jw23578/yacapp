#ifndef APPUSERCONFIG_H
#define APPUSERCONFIG_H

#include "configinterface.h"
#include "yacappmacros.h"
#include <QDateTime>

class AppUserConfig: public ConfigInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QString, loginEMail, LoginEMail, "");
    YACAPPPROPERTY(QString, loginToken, LoginToken, "");
    YACAPPPROPERTY(QDateTime, workStart, WorkStart, QDateTime());
    YACAPPPROPERTY(QDateTime, pauseStart, PauseStart, QDateTime());
    YACAPPPROPERTY(QDateTime, offSiteWorkStart, OffSiteWorkStart, QDateTime());
public:
    explicit AppUserConfig(QObject *parent = nullptr);

    void setConfig(const QJsonValue &config) override;
    QJsonObject getConfig() override;
};

#endif // APPUSERCONFIG_H
