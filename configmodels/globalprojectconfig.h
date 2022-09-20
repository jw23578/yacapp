#ifndef GLOBALPROJECTCONFIG_H
#define GLOBALPROJECTCONFIG_H

#include <QObject>
#include "yacappmacros.h"
#include "configinterface.h"

class GlobalProjectConfig : public ConfigInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QString, projectName, ProjectName, "")
    YACAPPPROPERTY(QString, mainFormFilename, MainFormFilename, "")
    public:
    explicit GlobalProjectConfig(QObject *parent = nullptr);

    QStringList formFiles;
    QStringList menueFiles;

    void setConfig(QJsonValue const &config) override;
    QJsonObject getConfig() override;

    void init(const QString &jsonConfigFile);
    void save(const QString &jsonConfigFile);
signals:

};

#endif // GLOBALPROJECTCONFIG_H
