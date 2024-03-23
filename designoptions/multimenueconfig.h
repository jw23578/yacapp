#ifndef MULTIMENUECONFIG_H
#define MULTIMENUECONFIG_H

#include <QObject>
#include "yacappmacros.h"
#include <QColor>

class MultiMenueConfig : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(QColor, color, Color, "orange")
    YACAPPPROPERTY(QColor, itemColor, ItemColor, "orange")
    YACAPPPROPERTY(QColor, borderColor, BorderColor, "darkorange")
    YACAPPPROPERTY(QColor, fontColor, FontColor, "#ffffff")
public:
    explicit MultiMenueConfig(QObject *parent = nullptr);

signals:

};

#endif // MULTIMENUECONFIG_H
