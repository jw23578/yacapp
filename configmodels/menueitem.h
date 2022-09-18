#ifndef MENUEITEM_H
#define MENUEITEM_H

#include <QObject>
#include "yacappmacros.h"

class MenueItem : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(QString, caption, Caption, "")
    YACAPPPROPERTY(QString, targetFile, TargetFile, "")
    YACAPPPROPERTY(QString, specialMeaning, SpecialMeaning, "")

public:
    explicit MenueItem(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config);

    QJsonObject getConfig();
signals:

};

#endif // MENUEITEM_H
