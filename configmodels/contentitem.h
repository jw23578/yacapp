#ifndef CONTENTITEM_H
#define CONTENTITEM_H

#include <QObject>
#include "yacappmacros.h"

class ContentItem : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(QString, filename, Filename, "")
    YACAPPPROPERTY(double, size, Size, 0)
public:
    explicit ContentItem(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config);
    QJsonObject getConfig();

signals:

};

#endif // CONTENTITEM_H
