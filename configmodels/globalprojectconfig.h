#ifndef GLOBALPROJECTCONFIG_H
#define GLOBALPROJECTCONFIG_H

#include <QObject>
#include "yacappmacros.h"
#include "configinterface.h"
#include "constants.h"
#include "appimagesitem.h"
#include <QQmlListProperty>
#include "datamodels/templateddatamodel.h"


class GlobalProjectConfig : public ConfigInterface
{
    Q_OBJECT
    YACAPPPROPERTY(int, version, Version, 0)
    YACAPPPROPERTY(QString, projectName, ProjectName, "")
    YACAPPPROPERTY(QString, projectID, ProjectID, "");
    YACAPPPROPERTY(QString, mainFormFilename, MainFormFilename, "")
    YACAPPPROPERTY(QString, logoUrl, LogoUrl, "")
    YACAPPPROPERTY(QString, appInfoUrl, AppInfoUrl, "")
    YACAPPPROPERTY(QString, searchCode, SearchCode, "")
    YACAPPPROPERTY(QString, projectColorName, ProjectColorName, "white");
    YACAPPPROPERTY(QString, third, Third, "");
    YACAPPPROPERTY(QString, mandant, Mandant, "");
    YACAPPPROPERTY(bool, appUserEnabled, AppUserEnabled, false);
    YACAPPPROPERTY(bool, appUserMessagesEnabled, AppUserMessagesEnabled, false);
    YACAPPPROPERTY(bool, appUserWorktimeEnabled, AppUserWorktimeEnabled, false);
    YACAPPPROPERTY(bool, appUserNewsEnabled, AppUserNewsEnabled, false);
    YACAPPPROPERTY(bool, appUserAppointmentsEnabled, AppUserAppointmentsEnabled, false);
    YACAPPPROPERTY(bool, appUserSpacesEnabled, AppUserSpacesEnabled, false);
    YACAPPPROPERTY(bool, appUserRightsEnabled, AppUserRightsEnabled, false);
    YACAPPPROPERTY(bool, isTemplateApp, IsTemplateApp, false);
    YACAPPPROPERTY(double, logoWidthPerThousand, LogoWidthPerThousand, 0)
    YACAPPPROPERTY(double, logoHeightPerThousand, LogoHeightPerThousand, 0)
    YACAPPPROPERTY(double, logoOffsetXPerThousand, LogoOffsetXPerThousand, 0)
    YACAPPPROPERTY(double, logoOffsetYPerThousand, LogoOffsetYPerThousand, 0)

//YACOBJECTLISTPROPERTY(MenueItem, item, Item, MenueConfig)
    YACAPP_MODEL_DECLARATION(TemplatedDataModel<AppImagesItem>, appImages);
    public:
    explicit GlobalProjectConfig(bool noProjectId = true,
                                 QObject *parent = nullptr);

    QStringList formFiles;
    QStringList menueFiles;

    void setConfig(QJsonValue const &config) override;
    QJsonObject getConfig() const override;
    QByteArray getConfigAsString(Constants &constants);

    void init(const QString &jsonConfigFile, Constants &constants);
    void save(const QString &jsonConfigFile, Constants &constants);
signals:

};

#endif // GLOBALPROJECTCONFIG_H
