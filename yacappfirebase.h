#ifndef YACAPPFIREBASE_H
#define YACAPPFIREBASE_H

#include "qglobal.h"

#ifdef Q_OS_ANDROID

#include "firebase/app.h"
#include "firebase/messaging.h"

#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroid>
#include "google_play_services/availability.h"


class MessageListener: public firebase::messaging::Listener
{

public:
    void OnMessage(const firebase::messaging::Message &message) override;
    void OnTokenReceived(const char *token) override;
};
class YACAPPFirebase
{
    MessageListener messageListener;
    firebase::App *firebaseApp;
    QAndroidJniEnvironment *qjniEnv;
    QAndroidJniObject activity;
    bool checkGooglePlayService();
public:
    YACAPPFirebase();
};

#else
#ifdef Q_OS_LINUX
class YACAPPFirebase
{
};
#endif
#endif



#endif // YACAPPFIREBASE_H
