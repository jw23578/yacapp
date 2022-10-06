#ifndef YACNETWORK_H
#define YACNETWORK_H

#include <QNetworkAccessManager>

class YACNetwork: public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    typedef std::function<void(QNetworkReply*)> replyCallbackFunction;
    QMap<QNetworkReply*, replyCallbackFunction> reply2callback;

    void projectFilenameFinished(QNetworkReply *reply);
public:
    YACNetwork();
    void fetchAllFiles(QString const &baseUrl, const QString &projectFilename, std::function<void()> allLoadedCallback);

public slots:
    void replyFinished(QNetworkReply *reply);
};

#endif // YACNETWORK_H
