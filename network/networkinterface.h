#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <QObject>
#include <QNetworkAccessManager>
#include "constants.h"

class NetworkInterface : public QObject
{
    Q_OBJECT
protected:
    QNetworkAccessManager &manager;
    const Constants &constants;
public:
    explicit NetworkInterface(QNetworkAccessManager &manager, const Constants &constants, QObject *parent = nullptr);

signals:

};

#endif // NETWORKINTERFACE_H
