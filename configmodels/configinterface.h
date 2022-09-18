#ifndef CONFIGINTERFACE_H
#define CONFIGINTERFACE_H

#include <QObject>

class ConfigInterface : public QObject
{
    Q_OBJECT
public:
    explicit ConfigInterface(QObject *parent = nullptr);

    bool isInit(QString const s);
    bool isInit(double const d);
    bool isInit(QColor const c);

    bool changed(QString const s);
    bool changed(double const d);
    bool changed(QColor const c);

    virtual void setConfig(const QJsonValue &config) = 0;
    virtual QJsonObject getConfig() = 0;

signals:

};

#endif // CONFIGINTERFACE_H
