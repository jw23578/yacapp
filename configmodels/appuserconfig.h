#ifndef APPUSERCONFIG_H
#define APPUSERCONFIG_H

#include <QObject>
#include "yacappmacros.h"

class AppUserConfig: public QObject
{
public:
    AppUserConfig(QObject *owner);
};

#endif // APPUSERCONFIG_H
