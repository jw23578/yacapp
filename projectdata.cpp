#include "projectdata.h"
#include <QQmlEngine>

ProjectData::ProjectData(QObject *parent)
    : QObject{parent}
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

