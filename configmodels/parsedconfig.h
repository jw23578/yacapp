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
    YACAPPREADONLYPROPERTY(BackgroundConfig*, background, new BackgroundConfig())
    YACAPPREADONLYPROPERTY(MenueConfig*, menue, new MenueConfig());
    YACAPPREADONLYPROPERTY(ContentConfig*, content, new ContentConfig());
    YACAPPREADONLYPROPERTY(HeaderConfig*, header, new HeaderConfig());
    YACAPPREADONLYPROPERTY(FooterConfig*, footer, new FooterConfig());
    YACAPPREADONLYPROPERTY(SplashscreenConfig*, splashscreen, new SplashscreenConfig());
    YACAPPPROPERTY(QString, menueFilename, MenueFilename, "");

public:
    explicit ParsedConfig(QObject *parent = nullptr);
    ~ParsedConfig();
    bool init(const QString &jsonConfigFile);

    void save(const QString &jsonConfigFile);

signals:

};

#endif // PARSEDCONFIG_H
