#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QDateTime>

class Helper : public QObject
{
    Q_OBJECT
    QString nullUuidString;
public:
    explicit Helper(QObject *parent = nullptr);       

    Q_INVOKABLE bool emailIsValid(const QString &email) const;

    Q_INVOKABLE bool passwordOk(const QString &password) const;

    Q_INVOKABLE QString formatTime(const QDateTime &dt) const;
    Q_INVOKABLE QString formatDateShort(const QDateTime &dt) const;
    Q_INVOKABLE QString formatDateLong(const QDateTime &dt) const;
    Q_INVOKABLE QString nameOfWeekDay(const QDateTime &dt) const;
    Q_INVOKABLE QString formatDateTime(const QDateTime &dt) const;

    Q_INVOKABLE QString smartFormatDateTime(const QDateTime &dt) const;

    Q_INVOKABLE QDateTime currentDateTime() const;

    Q_INVOKABLE QString getDayName(int day) const;
    Q_INVOKABLE QString getShortDayName(int day) const;

    Q_INVOKABLE QDateTime createDateTime(const int year, const int month, const int day, const int hour, const int minute) const;
    Q_INVOKABLE QDateTime createDateTime2(const QDateTime &datepart, const int hour, const int minute) const;
    Q_INVOKABLE int getHour(const QDateTime &dt) const;
    Q_INVOKABLE int getMinute(const QDateTime &dt) const;
    Q_INVOKABLE int getYear(const QDateTime &dt) const;
    Q_INVOKABLE int getMonth(const QDateTime &dt) const;
    Q_INVOKABLE int getDay(const QDateTime &dt) const;
    Q_INVOKABLE int getDayOfWeek(const QDateTime &dt) const;
    Q_INVOKABLE QDateTime setHour(const QDateTime &dt, int hour) const;
    Q_INVOKABLE QDateTime setMinute(const QDateTime &dt, int minute) const;

    Q_INVOKABLE QDateTime incDays(const QDateTime &dt, qint64 days) const;
    Q_INVOKABLE QDateTime incMonths(const QDateTime &dt, int months) const;
    Q_INVOKABLE QDateTime incYears(const QDateTime &dt, int years) const;

    Q_INVOKABLE bool sameDay(const QDateTime &a, const QDateTime &b) const;

    Q_INVOKABLE QDateTime firstInMonth(const QDateTime &dt) const;
    Q_INVOKABLE QDateTime lastInMonth(const QDateTime &dt) const;
    Q_INVOKABLE QString getMonthNameLong(int month) const;
    Q_INVOKABLE QString getMonthNameLongFromDate(const QDateTime &dt) const;
    Q_INVOKABLE int getDaysInMonth(int year, int month) const;
    Q_INVOKABLE int getDaysInMonthFromDate(const QDateTime &dt) const;

    Q_INVOKABLE int minutesBetween(const QDateTime &a, const QDateTime &b) const;

    Q_INVOKABLE bool isFolderEmpty(const QString &folder);

    Q_INVOKABLE void jsLog(const QString &message);

    Q_INVOKABLE QString generateUuid() const;
    Q_INVOKABLE bool validUuid(const QString &u) const;

signals:

};

#endif // HELPER_H
