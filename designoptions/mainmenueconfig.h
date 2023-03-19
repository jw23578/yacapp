#ifndef MAINMENUECONFIG_H
#define MAINMENUECONFIG_H

#include <QObject>
#include "yacappmacros.h"
#include <QColor>

class MainMenueConfig : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(double, smallElemHeightFactor, SmallElemHeightFactor, 79.0 / 932.0)
    YACAPPPROPERTY(double, smallElemCount, SmallElemCount, 3.5)
    YACAPPPROPERTY(QColor, smallElemColor, SmallElemColor, "#BFBCBC");
    YACAPPPROPERTY(QColor, smallFontColor, SmallFontColor, "#EEEEEE");
    YACAPPPROPERTY(QColor, largeElemColorLeft, largeElemColorLeft, "#D4D0D0")
    YACAPPPROPERTY(QColor, largeElemColorRight, largeElemColorRight, "#5E5757")
    YACAPPPROPERTY(double, largeElemHeightFactor, largeElemHeightFactor, 100.0 / 932.0)
    YACAPPPROPERTY(double, largeElemLeftWidthFactor, largeElemWidthFactor, 90.0 / 430.0)
    YACAPPPROPERTY(QColor, largeFontColor, LargeFontColor, "#FFFFFF");

    YACAPPPROPERTY(double, iconSizeFactor, IconSizeFactor, 21.0 / 430)
public:
    explicit MainMenueConfig(QObject *parent = nullptr);

signals:

};

#endif // MAINMENUECONFIG_H
