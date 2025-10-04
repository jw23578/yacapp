#ifndef PROJECTDATA_H
#define PROJECTDATA_H

#include <QObject>
#include "yacappmacros.h"

class ProjectData : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(QString, projectID, ProjectID, "");
    YACAPPPROPERTY(QString, projectName, ProjectName, "");
    YACAPPPROPERTY(QString, projectFilename, ProjectFilename, "");
    YACAPPPROPERTY(QString, logoUrl, LogoUrl, "");
    YACAPPPROPERTY(QString, installationCode, InstallationCode, "");
    YACAPPPROPERTY(QString, third, Third, "");
    YACAPPPROPERTY(QString, mandant, Mandant, "");

public:
    explicit ProjectData(QObject *parent = nullptr);

signals:

};

#endif // PROJECTDATA_H
