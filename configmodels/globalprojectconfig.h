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
    YACAPPPROPERTY(QString, projectColorName, ProjectColorName, "white");
    YACAPPPROPERTY(bool, isTemplateApp, IsTemplateApp, false);
    YACAPPPROPERTY(double, logoWidthPerThousand, LogoWidthPerThousand, 0)
    YACAPPPROPERTY(double, logoHeightPerThousand, LogoHeightPerThousand, 0)
    YACAPPPROPERTY(double, logoOffsetXPerThousand, LogoOffsetXPerThousand, 0)
    YACAPPPROPERTY(double, logoOffsetYPerThousand, LogoOffsetYPerThousand, 0)
    public:
    explicit GlobalProjectConfig(bool noProjectId = true,
                                 QObject *parent = nullptr);

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
