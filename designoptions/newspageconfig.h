#ifndef NEWSPAGECONFIG_H
#define NEWSPAGECONFIG_H

#include <QObject>
#include "yacappmacros.h"
#include <QColor>

class NewsPageConfig : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(double, topImageHeightFactor, TopImageHeightFactor, 250.0 / 932.0);
    YACAPPPROPERTY(double, topRightItemWidthFactor, topRightItemWidthFactor, 82.0 / 430.0);
    YACAPPPROPERTY(double, topRightItemHeightFactor, topRightItemHeightFactor, 65.0 / 932.0);
    YACAPPPROPERTY(double, contentItemWidthFactor, contentItemWidthFactor, 358.0 / 430.0);
    YACAPPPROPERTY(double, contentItemHeightFactor, contentItemHeightFactor, 120.0 / 932.0);
    YACAPPPROPERTY(double, contentItemSpacing, contentItemSpacing, 20.0 / 932.0);

    YACAPPPROPERTY(QColor, alertColor, AlertColor, "#B2ABAB");
    YACAPPPROPERTY(QColor, messagesColor, MessagesColor, "#CECACA");
public:
    explicit NewsPageConfig(QObject *parent = nullptr);
};

#endif // NEWSPAGECONFIG_H
