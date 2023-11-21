#include "helper.h"
#include <QRegularExpression>
#include <QDateTime>
#include <QLocale>
#include <QUrl>
#include <QDir>
#include <QUuid>
#include "logger.h"

Helper::Helper(QObject *parent)
    : QObject{parent},
    nullUuidString(QUuid().toString(QUuid::WithoutBraces))
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

QString Helper::getDayName(int day) const
{
    return QLocale::system().dayName(day);
}

QString Helper::getShortDayName(int day) const
{
    return QLocale::system().dayName(day, QLocale::ShortFormat);
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

int Helper::getDayOfWeek(const QDateTime &dt) const
{
    return dt.date().dayOfWeek();
}

QDateTime Helper::setHour(QDateTime const &dt, int hour) const
{
    return QDateTime(dt.date(), QTime(hour, dt.time().minute(), dt.time().second()));
}

QDateTime Helper::setMinute(const QDateTime &dt, int minute) const
{
    return QDateTime(dt.date(), QTime(dt.time().hour(), minute, dt.time().second()));
}

QDateTime Helper::incDays(const QDateTime &dt, qint64 days) const
{
    return dt.addDays(days);
}

QDateTime Helper::incMonths(const QDateTime &dt, int months) const
{
    return dt.addMonths(months);
}

QDateTime Helper::incYears(const QDateTime &dt, int years) const
{
    return dt.addYears(years);
}

bool Helper::sameDay(const QDateTime &a, const QDateTime &b) const
{
    return a.date() == b.date();
}

QDateTime Helper::firstInMonth(const QDateTime &dt) const
{
    return QDateTime(QDate(dt.date().year(), dt.date().month(), 1), QTime());
}

QDateTime Helper::lastInMonth(const QDateTime &dt) const
{
    return QDateTime(QDate(dt.date().year(), dt.date().month() + 1, 1).addDays(-1), QTime());
}

QString Helper::getMonthNameLong(int month) const
{
    return QLocale::system().monthName(month);
}

QString Helper::getMonthNameLongFromDate(const QDateTime &dt) const
{
    return getMonthNameLong(dt.date().month());
}

int Helper::getDaysInMonth(int year, int month) const
{
    return QDate(year, month, 1).daysInMonth();
}

int Helper::getDaysInMonthFromDate(const QDateTime &dt) const
{
    return getDaysInMonth(dt.date().year(), dt.date().month());
}

int Helper::minutesBetween(const QDateTime &a, const QDateTime &b) const
{
    return b.toSecsSinceEpoch() / 60 - a.toSecsSinceEpoch() / 60;
}

bool Helper::isFolderEmpty(const QString &folder)
{
    return QDir(QUrl(folder).path()).isEmpty();
}

void Helper::jsLog(const QString &message)
{
    JAVASCRIPT_LOG(message);
}

QString Helper::generateUuid() const
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

bool Helper::validUuid(const QString &u) const
{
    return u.size() > 0 && u != nullUuidString;
}
