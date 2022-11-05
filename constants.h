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
    YACAPPPROPERTY(bool, shadowActive, ShadowActive, true);
    YACAPPPROPERTY(int, radius, Radius, 21);
    YACAPPPROPERTY(int, shadowHorizontalOffset, ShadowHorizontalOffset, 5);
    YACAPPPROPERTY(int, shadowVerticalOffset, ShadowVerticalOffset, 5);
    YACAPPPROPERTY(bool, topLevelFormActive, TopLevelFormActive, false);
public:
    Constants();

    const QString &getWriteablePath() const;
    const QString getYacAppConfigPath() const;
    const QString getStateFilename() const;
};

#endif // CONSTANTS_H
