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

QString Helper::formatDateTime(const QDateTime &dt) const
{
    return formatDate(dt) + " " + formatTime(dt);
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

QDateTime Helper::currentDateTime() const
{
    return QDateTime::currentDateTime();
}

QDateTime Helper::createDateTime(const int year, const int month, const int day, const int hour, const int minute) const
{
    QDateTime result(QDate(year, month, day), QTime(hour, minute, 0, 0));
    return result;
}

QDateTime Helper::createDateTime2(const QDateTime &datepart, const int hour, const int minute) const
{
    return QDateTime(datepart.date(), QTime(hour, minute, 0, 0));
}

int Helper::getHour(const QDateTime &dt) const
{
    return dt.time().hour();
}

int Helper::getMinute(const QDateTime &dt) const
{
    return dt.time().minute();
}

int Helper::getYear(const QDateTime &dt) const
{
    return dt.date().year();
}

int Helper::getMonth(const QDateTime &dt) const
{
    return dt.date().month();
}

int Helper::getDay(const QDateTime &dt) const
{
    return dt.date().day();
}

bool Helper::sameDay(const QDateTime &a, const QDateTime &b) const
{
    return a.date() == b.date();
}

QString Helper::getMonthNameLong(int month) const
{
    return QLocale::system().monthName(month);
}

int Helper::getDaysInMonth(int year, int month) const
{
    return QDate(year, month, 1).daysInMonth();
}


