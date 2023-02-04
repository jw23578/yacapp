#include "constants.h"
#include <QStringList>
#include <QStandardPaths>
#include <QDir>
#include <QApplication>

Constants::Constants(const QString &customWriteablePath):writeablePath(customWriteablePath)
{
#ifdef Q_OS_WIN
    setIsDesktop(true);
#endif

#ifdef Q_OS_ANDROID
    setIsDesktop(false);
#else
#ifdef Q_OS_LINUX
    setIsDesktop(true);
#endif
#endif

    if (writeablePath == "")
    {
        QStringList paths(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        if (paths.size() == 0)
        {
            return;
        }
        writeablePath = paths[0] + "/";
    }
    QDir().mkpath(writeablePath);
}

const QString &Constants::getWriteablePath(QString appId)
{
    if (!writeablePathWidthAppId.contains(appId))
    {
        writeablePathWidthAppId = writeablePath + appId + "/";
        QDir().mkpath(writeablePathWidthAppId);
    }
    return writeablePathWidthAppId;
}
const QString Constants::getYacAppConfigPath(QString appId)
{
    QString path(getWriteablePath(appId) + "yacAppConfig/");
    QDir().mkpath(path);
    return path;
}

const QString Constants::getStateFilename() const
{
    return writeablePath + "yacAppState.json";
}

const QString Constants::getDBFilename(QString appId)
{
    return getWriteablePath(appId) + "yacApp.db";
}

const QString Constants::getAppConfigFilename(QString appId)
{
    return getWriteablePath(appId) + "appConfig.json";
}


