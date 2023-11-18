#include "yacappfirebase.h"
#include <QDebug>

bool YACAPPFirebase::checkGooglePlayService()
{
    QJniEnvironment env;
    QJniObject activity = QNativeInterface::QAndroidApplication::context();

    auto availablity = ::google_play_services::CheckAvailability(env.jniEnv(), activity.object());
    qDebug() << "GooglePlayServices::available() result :" << availablity << " (0 is kAvailabilityAvailable)";
    return true;
}

YACAPPFirebase::YACAPPFirebase(Firebase2Qt &firebase2qt):
    firebase2qt(firebase2qt),
    messageListener(firebase2qt)
{
    checkGooglePlayService();

    qjniEnv = new QJniEnvironment;

    QJniObject activity = QNativeInterface::QAndroidApplication::context();

    firebaseApp = ::firebase::App::GetInstance();
    if (!firebaseApp)
    {
        qDebug() << "creating firebase app";
        firebaseApp = firebase::App::Create(qjniEnv->jniEnv(), activity.object<jobject>());
    }
    if (firebaseApp)
    {
        qDebug() << "firebaseapp successfuly created";
    }

    qDebug() << "YACAPPFirebase";

    qDebug() << "-----------------------------------------*********************-------------------------";
    firebase::InitResult res = ::firebase::messaging::Initialize(*firebaseApp, &messageListener);
    qDebug() << "-----------------------------------------*********************-------------------------";
    qDebug() << __FILE__ << " " << __LINE__ << "Initresult: " << res;
    ::firebase::messaging::RequestPermission();
}


MessageListener::MessageListener(Firebase2Qt &firebase2qt):
    firebase2qt(firebase2qt)
{

}

MessageListener::~MessageListener()
{
    qDebug() << "destroy listener";
}

void MessageListener::OnMessage(const firebase::messaging::Message &message)
{
    qDebug() << "got message";
    for (auto const &s2s: message.data)
    {
        qDebug() << s2s.first.c_str() << ": " << s2s.second.c_str();
    }
    firebase2qt.handleNewMessage();
}

void MessageListener::OnTokenReceived(const char *token)
{
    const auto t = QString::fromUtf8(token);
    firebase2qt.handleDeviceToken(t);
    qDebug() << "from utf8: " << t;
    qDebug() << "got token: " << token;
}
