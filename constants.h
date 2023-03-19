#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>
#include <QString>
#include <QColor>
#include "yacappmacros.h"
#include "designoptions/mainmenueconfig.h"
#include "designoptions/newspageconfig.h"

class Constants : public QObject
{
    Q_OBJECT
    QString writeablePath;
    QString writeablePathWidthAppId;
    const QString theCachePath;
    YACAPPPROPERTY(int, appTotalWidth, AppTotalWidth, 0)
    YACAPPPROPERTY(int, appTotalHeight, AppTotalheight, 0)

    YACAPPPROPERTY(QString, allAppsBaseUrl, AllAppsBaseUrl, "https://www.jw78.de/yacapp/")
    YACAPPPROPERTY(bool, isDesktop, IsDesktop, false);
    YACAPPPROPERTY(bool, isConfigurator, IsConfigurator, false);
    YACAPPPROPERTY(bool, shadowActive, ShadowActive, true);
    YACAPPPROPERTY(int, radius, Radius, 21);
    YACAPPPROPERTY(int, shadowHorizontalOffset, ShadowHorizontalOffset, 0); // 5);
    YACAPPPROPERTY(int, shadowVerticalOffset, ShadowVerticalOffset, 5);
    YACAPPPROPERTY(int, shadowSamples, ShadowSamples, 9)
    YACAPPPROPERTY(bool, topLevelFormActive, TopLevelFormActive, true);
    YACAPPPROPERTY(bool, profileOpen, ProfileOpen, false);
    YACAPPPROPERTY(bool, superMenueOpen, SuperMenueOpen, false);
    YACAPPPROPERTY(bool, superMenueClicked, SuperMenueClicked, false);
    YACAPPPROPERTY(double, defaultWidthFactor, DefaultWidthFactor, 6.0 / 7.0)

    YACAPPPROPERTY(int, fastAnimationDuration, FastAnimationDuration, 200)
    YACAPPPROPERTY(int, slowAnimationDuration, SlowAnimationDuration, 400)

    YACAPPPROPERTY(double, defaultMargin, DefaultMargin, 0);

    YACAPPPROPERTY(double, defaultFontPixelSize, DefaultFontPixelSize, 0)
    YACAPPPROPERTY(double, smallerTextFactor, SmallerTextFactor, 3.0 / 4.0)
    YACAPPPROPERTY(double, largerTextFactor, LargerTextFactor, 5.0 / 4.0)
    YACAPPPROPERTY(double, x2largerTextFactor, X2largerTextFactor, 6.0 / 4.0)
    YACAPPPROPERTY(double, x3largerTextFactor, X3largerTextFactor, 7.0 / 4.0)
    YACAPPPROPERTY(double, x4largerTextFactor, X4largerTextFactor, 8.0 / 4.0)

    YACAPPPROPERTY(QColor, goodColor, GoodColor, "#A9CEA5");
    YACAPPPROPERTY(QColor, badColor, BadColor, "#F09494");

    YACAPPPROPERTY(MainMenueConfig*, mainMenueConfig, MainMenueConfig, new MainMenueConfig);
    YACAPPPROPERTY(NewsPageConfig*, newsPageConfig, NewsPageConfig, new NewsPageConfig);
public:
    Constants() {}
    Constants(const QString &customWriteablePath);

    const QString &getWriteablePath(QString appId);
    const QString getYacAppConfigPath(QString appId);
    const QString getStateFilename() const;
    const QString getDBFilename(QString appId);
    const QString getAppConfigFilename(QString appId);
    const QString getCachePath() const;

};

#endif // CONSTANTS_H
