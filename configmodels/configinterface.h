#ifndef CONFIGINTERFACE_H
#define CONFIGINTERFACE_H

#include <QObject>
#include "yacappmacros.h"

class ConfigInterface : public QObject
{
    Q_OBJECT
public:
    explicit ConfigInterface(QObject *parent = nullptr);

    bool isInit(QString const s) const;
    bool isInit(double const d) const;
    bool isInit(QColor const c) const;

    bool changed(QString const s) const;
    bool changed(double const d) const;
    bool changed(QColor const c) const;

    virtual void setConfig(const QJsonValue &config) = 0;
    virtual QJsonObject getConfig() const = 0;

signals:

};

#endif // CONFIGINTERFACE_H
