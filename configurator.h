#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QObject>
#include "yacappmacros.h"
#include "projectdata.h"
#include <QMap>

class Configurator : public QObject
{
    Q_OBJECT
    QString configFilename;

    YACAPPPROPERTY(QString, lastProjectName, LastProjectName, "");
    YACAPPPROPERTY(QString, lastProjectFilename, LastProjectFilename, "");

    QMap<QString, ProjectData*> deployConfigs;

    void sftpUpload(QString host, QString user, QString password, QString targetFilename, QString sourceFilename);

public:
    explicit Configurator(QObject *parent = nullptr);

    Q_INVOKABLE void save();

    Q_INVOKABLE void deploy(QString projectID, QString host, QString user, QString password, QString www_basedirectory);

    Q_INVOKABLE void defaultDeploy(const QString &globalProjectConfigFilename, QString host, QString user, QString password);

    Q_INVOKABLE ProjectData *getProjectData(const QString &projectID);

signals:

};

#endif // CONFIGURATOR_H
