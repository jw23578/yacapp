#include "firebase2qt.h"

Firebase2Qt::Firebase2Qt(QObject *parent)
    : QObject{parent}
{

}

void Firebase2Qt::handleDeviceToken(QString deviceToken)
{
    emit deviceTokenChanged(deviceToken);
}

void Firebase2Qt::handleNewMessage()
{
    emit newMessages();
}

