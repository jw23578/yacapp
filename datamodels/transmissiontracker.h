#ifndef TRANSMISSIONTRACKER_H
#define TRANSMISSIONTRACKER_H

#include <QObject>
#include "yacappmacros.h"

class TransmissionTracker : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(int, percentState, PercentState, 0);
    YACAPPPROPERTY(bool, active, Active, false);
public:
    explicit TransmissionTracker(QObject *parent = nullptr);

signals:
public slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
};

#endif // TRANSMISSIONTRACKER_H
