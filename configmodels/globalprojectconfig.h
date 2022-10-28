#ifndef GLOBALPROJECTCONFIG_H
#define GLOBALPROJECTCONFIG_H

#include <QObject>
#include "yacappmacros.h"
#include "configinterface.h"

class GlobalProjectConfig : public ConfigInterface
{
    Q_OBJECT
    YACAPPPROPERTY(int, version, Version, 0)
    YACAPPPROPERTY(QString, projectName, ProjectName, "")
    YACAPPPROPERTY(QString, projectID, ProjectID, "");
    YACAPPPROPERTY(QString, mainFormFilename, MainFormFilename, "")
    YACAPPPROPERTY(QString, logoUrl, LogoUrl, "")
    YACAPPPROPERTY(double, logoWidth, LogoWidth, 0)
    YACAPPPROPERTY(double, logoHeight, LogoHeight, 0)
    YACAPPPROPERTY(double, logoOffsetX, LogoOffsetX, 0)
    YACAPPPROPERTY(double, logoOffsetY, LogoOffsetY, 0)
    public:
    explicit GlobalProjectConfig(QObject *parent = nullptr);

    QStringList formFiles;
    QStringList menueFiles;

    void setConfig(QJsonValue const &config) override;
    QJsonObject getConfig() override;
    QByteArray getConfigAsString();

    void init(const QString &jsonConfigFile);
    void save(const QString &jsonConfigFile);
signals:

};

#endif // GLOBALPROJECTCONFIG_H
