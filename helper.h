#ifndef HELPER_H
#define HELPER_H

#include <QObject>

class Helper : public QObject
{
    Q_OBJECT
public:
    explicit Helper(QObject *parent = nullptr);

    Q_INVOKABLE bool emailIsValid(const QString &email);

signals:

};

#endif // HELPER_H
