#ifndef THIRDPARTYLOGIN_H
#define THIRDPARTYLOGIN_H

#include <QObject>
#include "yacappmacros.h"

class ThirdPartyLogin : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(QString, loginToken, LoginToken, "");
public:
    explicit ThirdPartyLogin(QObject *parent = nullptr);

signals:

};

#endif // THIRDPARTYLOGIN_H
