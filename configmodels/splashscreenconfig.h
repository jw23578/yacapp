#ifndef SPLASHSCREENCONFIG_H
#define SPLASHSCREENCONFIG_H

#include <QObject>
#include "yacappmacros.h"

class SplashscreenConfig : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(int, secondsDuration, SecondsDuration, 0)
    YACAPPPROPERTY(QString, imageFilename, ImageFilename, "")
    public:
    explicit SplashscreenConfig(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config);
    QJsonObject getConfig();

signals:

};

#endif // SPLASHSCREENCONFIG_H
