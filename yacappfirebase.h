#ifndef YACAPPFIREBASE_H
#define YACAPPFIREBASE_H

#include "qglobal.h"

#include "firebase2qt.h"

#ifdef Q_OS_ANDROID

#include "firebase/app.h"
#include "firebase/messaging.h"

#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroid>
#include "google_play_services/availability.h"



class MessageListener: public firebase::messaging::Listener
{
    Firebase2Qt &firebase2qt;
public:
    MessageListener(Firebase2Qt &firebase2qt);
    ~MessageListener();
    void OnMessage(const firebase::messaging::Message &message) override;
    void OnTokenReceived(const char *token) override;
};

class YACAPPFirebase
{
    Firebase2Qt &firebase2qt;
    MessageListener messageListener;
    firebase::App *firebaseApp;
    QAndroidJniEnvironment *qjniEnv;
    QAndroidJniObject activity;
    bool checkGooglePlayService();
public:
    YACAPPFirebase(Firebase2Qt &firebase2qt);
};

#else
#if defined Q_OS_LINUX || defined Q_OS_WINDOWS
class YACAPPFirebase
{
    Firebase2Qt &firebase2qt;
public:
    YACAPPFirebase(Firebase2Qt &firebase2qt):firebase2qt(firebase2qt) {}
};
#endif
#endif



#endif // YACAPPFIREBASE_H
