#ifndef YACAPP_H
#define YACAPP_H

#include <QObject>
#include "parsedconfig.h"

class YACAPP : public QObject
{
    Q_OBJECT
    YACAPPREADONLYPROPERTY(ParsedConfig*, mainConfig, new ParsedConfig);
    QMap<QString, ParsedConfig*> fileName2ParsedConfig;
public:
    explicit YACAPP(QObject *parent = nullptr);

    Q_INVOKABLE ParsedConfig *getConfig(QString const &fileName);

signals:

};

#endif // YACAPP_H
