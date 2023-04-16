#ifndef RECENTPROJECT_H
#define RECENTPROJECT_H

#include "yacappmacros.h"
#include <configmodels/configinterface.h>

class RecentProject : public ConfigInterface
{
    Q_OBJECT

    YACAPPPROPERTY(QString, projectID, ProjectID, "");
    YACAPPPROPERTY(QString, projectName, ProjectName, "")
    YACAPPPROPERTY(QString, logoUrl, LogoUrl, "")
    YACAPPPROPERTY(QString, projectColorName, ProjectColorName, "white");
public:
    explicit RecentProject(QObject *parent = nullptr);

    void setConfig(const QJsonValue &config) override;
    QJsonObject getConfig() const override;
};

#endif // RECENTPROJECT_H
