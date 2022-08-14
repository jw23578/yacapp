#ifndef BACKGROUNDCONFIG_H
#define BACKGROUNDCONFIG_H

#include <QObject>
#include <QColor>
#include "yacappmacros.h"


class BackgroundConfig: public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(QColor, color, Color, "black")
    YACAPPPROPERTY(QString, imageFilename, ImageFilename, "test")

public:
    explicit BackgroundConfig(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config);
    QJsonObject getConfig();

signals:

};

#endif // BACKGROUNDCONFIG_H
