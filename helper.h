#ifndef HELPER_H
#define HELPER_H

#include <QObject>

class Helper : public QObject
{
    Q_OBJECT
public:
    explicit Helper(QObject *parent = nullptr);

    Q_INVOKABLE bool emailIsValid(const QString &email) const;

    Q_INVOKABLE bool passwordOk(const QString &password) const;

    Q_INVOKABLE QString formatTime(const QDateTime &dt) const;
    Q_INVOKABLE QString formatDate(const QDateTime &dt) const;

signals:

};

#endif // HELPER_H
