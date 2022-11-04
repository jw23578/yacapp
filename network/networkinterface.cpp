#include "networkinterface.h"

NetworkInterface::NetworkInterface(QNetworkAccessManager &manager
                                   , const Constants &constants
                                   , QObject *parent)
    : QObject{parent}
    , manager(manager)
    , constants(constants)
{

}
