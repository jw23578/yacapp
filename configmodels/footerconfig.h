#ifndef FOOTERCONFIG_H
#define FOOTERCONFIG_H

#include <QObject>
#include "yacappmacros.h"
#include <QColor>

class FooterConfig : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(double, height, Height, 0)
    YACAPPPROPERTY(QColor, color, Color, "white")
public:
    explicit FooterConfig(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config);
    QJsonObject getConfig();
signals:

};

#endif // FOOTERCONFIG_H
