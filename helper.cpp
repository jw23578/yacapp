#include "helper.h"
#include <QRegularExpression>
#include <QDateTime>
#include <QLocale>
Helper::Helper(QObject *parent)
    : QObject{parent}
{

}

bool Helper::emailIsValid(const QString &email) const
{
    static const QRegularExpression regex("^[_a-zA-Z0-9-]+(\\.[_a-zA-Z0-9-]+)*@[a-z0-9-]+(\\.[a-z0-9-]+)*(\\.[a-z]{2,63})$");
    return regex.match(email).hasMatch();
}

bool Helper::passwordOk(const QString &password) const
{
    if (password.size() < 4)
    {
        return false;
    }
    return true;
}

QString Helper::formatTime(const QDateTime &dt) const
{
    return dt.toString(QLocale::system().timeFormat(QLocale::ShortFormat));
}

QString Helper::formatDate(const QDateTime &dt) const
{
    return dt.toString(QLocale::system().dateFormat(QLocale::ShortFormat));
}

QString Helper::smartFormatDateTime(const QDateTime &dt) const
{
    QString s;
    if (dt.date() < QDateTime::currentDateTime().date())
    {
        s += formatDate(dt);
        s += " ";
    }
    s += formatTime(dt);
    return s;
}
