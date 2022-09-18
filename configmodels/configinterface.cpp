#include "configinterface.h"
#include <QColor>

ConfigInterface::ConfigInterface(QObject *parent)
    : QObject{parent}
{

}

bool ConfigInterface::isInit(const QString s)
{
    return s == "";
}

bool ConfigInterface::isInit(const double d)
{
    return d == 0;
}

bool ConfigInterface::isInit(const QColor c)
{
    return !c.isValid() || c == Qt::black;
}

bool ConfigInterface::changed(const QString s)
{
    return !isInit(s);
}

bool ConfigInterface::changed(const double d)
{
    return !isInit(d);
}

bool ConfigInterface::changed(const QColor c)
{
    return !isInit(c);
}
