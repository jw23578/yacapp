#ifndef BACKGROUNDCONFIG_H
#define BACKGROUNDCONFIG_H

#include <QObject>
#include <QColor>
#include "configinterface.h"


class BackgroundConfig: public ConfigInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QColor, color, Color, "black")
    YACAPPPROPERTY(QString, imageFilename, ImageFilename, "test")

public:
    explicit BackgroundConfig(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config) override;
    QJsonObject getConfig() const override;

signals:

};

#endif // BACKGROUNDCONFIG_H
