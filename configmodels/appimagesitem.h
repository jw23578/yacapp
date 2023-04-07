#ifndef APPIMAGESITEM_H
#define APPIMAGESITEM_H

#include "configinterface.h"

class AppImagesItem : public ConfigInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QString, fileUrl, FileUrl, "");
public:
    explicit AppImagesItem(QObject *parent = nullptr);

    void setConfig(const QJsonValue &config) override;
    QJsonObject getConfig() override;

    void assign(const AppImagesItem &other);
};

#endif // APPIMAGESITEM_H
