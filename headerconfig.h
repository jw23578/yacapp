#ifndef HEADERCONFIG_H
#define HEADERCONFIG_H

#include <QObject>
#include "yacappmacros.h"
#include <QColor>

class HeaderConfig : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(QString, title, Title, "")
    YACAPPPROPERTY(double, height, Height, 0)
    YACAPPPROPERTY(QColor, color, Color, "white")
public:
    explicit HeaderConfig(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config);
signals:

};

#endif // HEADERCONFIG_H
