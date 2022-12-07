#include "dataobjectinterface.h"
#include <QQmlEngine>

DataObjectInterface::DataObjectInterface(QObject *parent)
    : QObject{parent}
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}
