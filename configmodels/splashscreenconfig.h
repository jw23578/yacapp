#ifndef SPLASHSCREENCONFIG_H
#define SPLASHSCREENCONFIG_H

#include <QObject>
#include "yacappmacros.h"
#include "configinterface.h"

class SplashscreenConfig : public ConfigInterface
{
    Q_OBJECT
    YACAPPPROPERTY(int, secondsDuration, SecondsDuration, 0)
    YACAPPPROPERTY(QString, imageFilename, ImageFilename, "")
    public:
    explicit SplashscreenConfig(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config) override;
    QJsonObject getConfig() const override;

signals:

};

#endif // SPLASHSCREENCONFIG_H
