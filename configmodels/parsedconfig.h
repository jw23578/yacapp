#ifndef PARSEDCONFIG_H
#define PARSEDCONFIG_H

#include <QObject>
#include <QJsonDocument>
#include "yacappmacros.h"
#include "backgroundconfig.h"
#include "menueconfig.h"
#include "contentconfig.h"
#include "headerconfig.h"
#include "footerconfig.h"
#include "splashscreenconfig.h"

class ParsedConfig : public QObject
{
    Q_OBJECT
    QJsonDocument config;

    YACAPPREADONLYPROPERTY(BackgroundConfig*, background, new BackgroundConfig())
    YACAPPREADONLYPROPERTY(MenueConfig*, menue, new MenueConfig());
    YACAPPREADONLYPROPERTY(ContentConfig*, content, new ContentConfig());
    YACAPPREADONLYPROPERTY(HeaderConfig*, header, new HeaderConfig());
    YACAPPREADONLYPROPERTY(FooterConfig*, footer, new FooterConfig());
    YACAPPREADONLYPROPERTY(SplashscreenConfig*, splashscreen, new SplashscreenConfig());
    YACAPPPROPERTY(QString, menueFilename, MenueFilename, "");
    public:
        explicit ParsedConfig(QObject *parent = nullptr);
    bool init(const QString &jsonConfigFile,
              const QString &baseUrl);

    void save(const QString &jsonConfigFile,
              const QString &baseUrl);

signals:

};

#endif // PARSEDCONFIG_H
