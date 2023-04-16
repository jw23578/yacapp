#include "configinterface.h"
#include <QColor>
#include <QQmlEngine>

ConfigInterface::ConfigInterface(QObject *parent)
    : QObject{parent}
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

bool ConfigInterface::isInit(const QString s) const
{
    return s == "";
}

bool ConfigInterface::isInit(const double d) const
{
    return d == 0;
}

bool ConfigInterface::isInit(const QColor c) const
{
    return !c.isValid() || c == Qt::black;
}

bool ConfigInterface::changed(const QString s) const
{
    return !isInit(s);
}

bool ConfigInterface::changed(const double d) const
{
    return !isInit(d);
}

bool ConfigInterface::changed(const QColor c) const
{
    return !isInit(c);
}
