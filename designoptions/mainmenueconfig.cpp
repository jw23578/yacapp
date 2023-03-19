#include "mainmenueconfig.h"
#include <QQmlEngine>

MainMenueConfig::MainMenueConfig(QObject *parent)
    : QObject{parent}
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}
