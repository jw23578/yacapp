#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>
#include <QString>
#include "yacappmacros.h"

class Constants : public QObject
{
    Q_OBJECT
    QString writeablePath;
    YACAPPPROPERTY(QString, allAppsBaseUrl, AllAppsBaseUrl, "https://www.jw78.de/yacapp/")
    YACAPPPROPERTY(bool, isDesktop, IsDesktop, false);
    YACAPPPROPERTY(bool, isConfigurator, IsConfigurator, false);
public:
    Constants();

    const QString &getWriteablePath() const;
    const QString getYacAppConfigPath() const;
    const QString getStateFilename() const;
};

#endif // CONSTANTS_H
