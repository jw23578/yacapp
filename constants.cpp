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

#ifdef Q_OS_LINUX
    setIsDesktop(true);
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

    QDir().mkpath(getYacAppConfigPath());
}

const QString &Constants::getWriteablePath() const
{
    return writeablePath;
}
const QString Constants::getYacAppConfigPath() const
{
    return writeablePath + "yacAppConfig/";
}

const QString Constants::getStateFilename() const
{
    return writeablePath + "yacAppState.json";
}

const QString Constants::getDBFilename() const
{
    return writeablePath + "yacApp.db";
}


