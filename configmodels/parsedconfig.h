#ifndef PARSEDCONFIG_H
#define PARSEDCONFIG_H

#include <QObject>
#include <QJsonDocument>
#include "yacappmacros.h"
#include "backgroundconfig.h"
#include "menueconfig.h"
#include "contentconfig.h"
#include "headerconfig.h"
#include "splashscreenconfig.h"

class ParsedConfig : public QObject
{
    Q_OBJECT
    YACAPP_READONLY_PROPERTY(BackgroundConfig*, background, new BackgroundConfig())
    YACAPP_READONLY_PROPERTY(MenueConfig*, menue, new MenueConfig());
    YACAPP_READONLY_PROPERTY(ContentConfig*, content, new ContentConfig());
    YACAPP_READONLY_PROPERTY(HeaderConfig*, header, new HeaderConfig());
    YACAPP_READONLY_PROPERTY(SplashscreenConfig*, splashscreen, new SplashscreenConfig());
    YACAPPPROPERTY(QString, menueFilename, MenueFilename, "");

public:
    explicit ParsedConfig(QObject *parent = nullptr);
    ~ParsedConfig();
    bool init(const QString &jsonConfigFile);

    void save(const QString &jsonConfigFile);

signals:

};

#endif // PARSEDCONFIG_H
