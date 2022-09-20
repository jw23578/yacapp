#ifndef GLOBALPROJECTCONFIG_H
#define GLOBALPROJECTCONFIG_H

#include <QObject>
#include "yacappmacros.h"

class GlobalProjectConfig : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(QString, projectName, ProjectName, "")
    YACAPPPROPERTY(QString, mainFormFilename, MainFormFilename, "")
    public:
    explicit GlobalProjectConfig(QObject *parent = nullptr);

    QStringList formFiles;
    QStringList menueFiles;

    void setConfig(QJsonValue const &config);
    void getConfig(QJsonObject &config);

    void init(const QString &jsonConfigFile);
    void save(const QString &jsonConfigFile);
signals:

};

#endif // GLOBALPROJECTCONFIG_H
