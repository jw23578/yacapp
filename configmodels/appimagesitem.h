#ifndef APPIMAGESITEM_H
#define APPIMAGESITEM_H

#include "configinterface.h"
#include "definitions.h"

class AppImagesItem : public ConfigInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QString, fileUrl, FileUrl, "");
    YACAPPPROPERTY(ORMUuid, imageId, ImageId, "");
public:
    explicit AppImagesItem(QObject *parent = nullptr);

    void setConfig(const QJsonValue &config) override;
    QJsonObject getConfig() const override;

    void assign(const AppImagesItem &other);
};

#endif // APPIMAGESITEM_H
