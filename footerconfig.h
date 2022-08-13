#ifndef FOOTERCONFIG_H
#define FOOTERCONFIG_H

#include <QObject>
#include "yacappmacros.h"

class FooterConfig : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(double, height, Height, 0)
public:
    explicit FooterConfig(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config);
signals:

};

#endif // FOOTERCONFIG_H
