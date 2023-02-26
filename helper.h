#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QDateTime>

class Helper : public QObject
{
    Q_OBJECT
public:
    explicit Helper(QObject *parent = nullptr);

    Q_INVOKABLE bool emailIsValid(const QString &email) const;

    Q_INVOKABLE bool passwordOk(const QString &password) const;

    Q_INVOKABLE QString formatTime(const QDateTime &dt) const;
    Q_INVOKABLE QString formatDate(const QDateTime &dt) const;
    Q_INVOKABLE QString formatDateTime(const QDateTime &dt) const;

    Q_INVOKABLE QString smartFormatDateTime(const QDateTime &dt) const;

    Q_INVOKABLE QDateTime currentDateTime() const;


    Q_INVOKABLE QDateTime createDateTime(const int year, const int month, const int day, const int hour, const int minute) const;
    Q_INVOKABLE QDateTime createDateTime2(const QDateTime &datepart, const int hour, const int minute) const;
    Q_INVOKABLE int getHour(const QDateTime &dt) const;
    Q_INVOKABLE int getMinute(const QDateTime &dt) const;
    Q_INVOKABLE int getYear(const QDateTime &dt) const;
    Q_INVOKABLE int getMonth(const QDateTime &dt) const;
    Q_INVOKABLE int getDay(const QDateTime &dt) const;

    Q_INVOKABLE bool sameDay(const QDateTime &a, const QDateTime &b) const;

    Q_INVOKABLE QString getMonthNameLong(int month) const;
    Q_INVOKABLE int getDaysInMonth(int year, int month) const;

signals:

};

#endif // HELPER_H
