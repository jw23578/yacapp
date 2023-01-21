#include "yacappfirebase.h"
#include <QDebug>

bool YACAPPFirebase::checkGooglePlayService()
{
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = QtAndroid::androidActivity();

    auto availablity = ::google_play_services::CheckAvailability(env, activity.object());
    qDebug() << "GooglePlayServices::available() result :" << availablity << " (0 is kAvailabilityAvailable)";
    return true;
}

YACAPPFirebase::YACAPPFirebase()
{
    checkGooglePlayService();

    qjniEnv = new QAndroidJniEnvironment;

    QAndroidJniObject activity = QtAndroid::androidActivity();

    firebaseApp = ::firebase::App::GetInstance();
    if (!firebaseApp)
    {
        qDebug() << "creating firebase app";
        firebaseApp = firebase::App::Create(*qjniEnv, activity.object<jobject>());
    }

    qDebug() << "YACAPPFirebase";

    ::firebase::messaging::RequestPermission();
    qDebug() << "-----------------------------------------*********************-------------------------";
    firebase::InitResult res = ::firebase::messaging::Initialize(*firebaseApp, &messageListener);
    qDebug() << "-----------------------------------------*********************-------------------------";
    qDebug() << __FILE__ << " " << __LINE__ << "Initresult: " << res;
}


void MessageListener::OnMessage(const firebase::messaging::Message &message)
{
   qDebug() << "got message";
   for (auto const &s2s: message.data)
   {
       qDebug() << s2s.first.c_str() << ": " << s2s.second.c_str();
   }
}

void MessageListener::OnTokenReceived(const char *token)
{
    const auto t = QString::fromUtf8(token);
    qDebug() << "from utf8: " << t;
    qDebug() << "got token: " << token;
}
