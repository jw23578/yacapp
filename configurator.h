#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QObject>
#include "yacappmacros.h"

class Configurator : public QObject
{
    Q_OBJECT
    QString configFilename;

    YACAPPPROPERTY(QString, lastProjectName, LastProjectName, "");
    YACAPPPROPERTY(QString, lastProjectFilename, LastProjectFilename, "");
public:
    explicit Configurator(QObject *parent = nullptr);

    Q_INVOKABLE void save();

    Q_INVOKABLE void deploy(QString host, QString user, QString password, QString www_basedirectory);

signals:

};

#endif // CONFIGURATOR_H
