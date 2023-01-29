#ifndef FIREBASE2QT_H
#define FIREBASE2QT_H

#include <QObject>

class Firebase2Qt : public QObject
{
    Q_OBJECT
public:
    explicit Firebase2Qt(QObject *parent = nullptr);

    void handleDeviceToken(QString deviceToken);
    void handleNewMessage();
signals:
    void deviceTokenChanged(QString deviceToken);
    void newMessages();
};

#endif // FIREBASE2QT_H
