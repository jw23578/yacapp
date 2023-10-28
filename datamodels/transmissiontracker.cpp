#include "transmissiontracker.h"
#include <QQmlEngine>

TransmissionTracker::TransmissionTracker(QObject *parent)
    : QObject{parent}
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

void TransmissionTracker::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (!bytesTotal)
    {
        setPercentState(100);
        return;
    }
    setPercentState(100 * bytesReceived / bytesTotal);
}

void TransmissionTracker::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    if (!bytesTotal)
    {
        setPercentState(100);
        return;
    }
    setPercentState(100 * bytesSent / bytesTotal);
}
