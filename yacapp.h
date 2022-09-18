#ifndef YACAPP_H
#define YACAPP_H

#include <QObject>
#include "configmodels/parsedconfig.h"
#include "configmodels/globalprojectconfig.h"

class YACAPP : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(QString, baseUrl, BaseUrl, "");
    YACAPPREADONLYPROPERTY(ParsedConfig*, mainConfig, new ParsedConfig);
    YACAPPREADONLYPROPERTY(GlobalProjectConfig*, globalConfig, new GlobalProjectConfig);
    YACAPPPROPERTY(QStringList, knownFiles, KnownFiles, QStringList())
    QMap<QString, ParsedConfig*> fileName2ParsedConfig;
    void addKnownFile(QString const &filename);

    YACAPPPROPERTY(QStringList, knownMenueFiles, KnownMenueFiles, QStringList())
    QMap<QString, MenueConfig*> fileName2MenueConfig;
    void addKnownMenueFile(QString const &filename);

    MenueConfig emptyMenue;
public:
    explicit YACAPP(QObject *parent = nullptr);
    void init();

    Q_INVOKABLE ParsedConfig *getConfig(QString const &filename);

    Q_INVOKABLE MenueConfig *getMenueConfig(QString const &filename);

    Q_INVOKABLE void loadNewProject(QString const &folder);

    Q_INVOKABLE void saveCurrentProject();

signals:

};

#endif // YACAPP_H
