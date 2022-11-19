#ifndef PROJECTDATA_H
#define PROJECTDATA_H

#include <QObject>
#include "yacappmacros.h"

class ProjectData : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(QString, projectID, ProjectID, "");
    YACAPPPROPERTY(QString, projectName, ProjectName, "");
    YACAPPPROPERTY(QString, logoUrl, LogoUrl, "");
    YACAPPPROPERTY(QString, deployUrl, DeployUrl, "");
    YACAPPPROPERTY(QString, deployUser, DeployUser, "");
    YACAPPPROPERTY(QString, deployPassword, DeployPassword, "");
    YACAPPPROPERTY(QString, deployBaseDirectory, DeployBaseDirectory, "");

    YACAPPPROPERTY(QString, yacappServerLoginToken, YacappServerLoginToken, "");
public:
    explicit ProjectData(QObject *parent = nullptr);

signals:

};

#endif // PROJECTDATA_H
