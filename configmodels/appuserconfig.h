#ifndef APPUSERCONFIG_H
#define APPUSERCONFIG_H

#include "configinterface.h"
#include "yacappmacros.h"

class AppUserConfig: public ConfigInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QString, loginToken, LoginToken, "");
public:
    explicit AppUserConfig(QObject *parent = nullptr);

    void setConfig(const QJsonValue &config) override;
    QJsonObject getConfig() override;
};

#endif // APPUSERCONFIG_H
