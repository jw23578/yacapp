#ifndef MULTIMENUECONFIG_H
#define MULTIMENUECONFIG_H

#include <QObject>
#include "yacappmacros.h"
#include <QColor>

class MultiMenueConfig : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(QColor, color, Color, "#5E5757")
    YACAPPPROPERTY(QColor, itemColor, ItemColor, "#B2A6A6")
    YACAPPPROPERTY(QColor, fontColor, FontColor, "#ffffff")
public:
    explicit MultiMenueConfig(QObject *parent = nullptr);

signals:

};

#endif // MULTIMENUECONFIG_H
