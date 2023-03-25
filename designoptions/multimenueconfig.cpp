#include "multimenueconfig.h"
#include <QQmlEngine>

MultiMenueConfig::MultiMenueConfig(QObject *parent)
    : QObject{parent}
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}
