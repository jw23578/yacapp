#include "helper.h"
#include <QRegularExpression>

Helper::Helper(QObject *parent)
    : QObject{parent}
{

}

bool Helper::emailIsValid(const QString &email)
{
    static const QRegularExpression regex("^[_a-zA-Z0-9-]+(\\.[_a-zA-Z0-9-]+)*@[a-z0-9-]+(\\.[a-z0-9-]+)*(\\.[a-z]{2,63})$");
    return regex.match(email).hasMatch();
}
