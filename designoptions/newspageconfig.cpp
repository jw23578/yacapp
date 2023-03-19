#include "newspageconfig.h"
#include <QQmlEngine>

NewsPageConfig::NewsPageConfig(QObject *parent)
    : QObject{parent}
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}
