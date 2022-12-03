#include "appuserconfig.h"
#include <QQmlEngine>

AppUserConfig::AppUserConfig(QObject *owner):
    QObject(owner)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}
