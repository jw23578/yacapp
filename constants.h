#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

class Constants
{
    QString writeablePath;
public:
    Constants();

    const QString &getWriteablePath() const;
    const QString getYacAppConfigPath() const;
    const QString getStateFilename() const;
};

#endif // CONSTANTS_H
