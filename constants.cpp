#include "constants.h"
#include <QStringList>
#include <QStandardPaths>
#include <QDir>

Constants::Constants()
{
    QStringList paths(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (paths.size() == 0)
    {
        return;
    }
    writeablePath = paths[0] + "/";
    QDir().mkdir(getYacAppConfigPath());
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


