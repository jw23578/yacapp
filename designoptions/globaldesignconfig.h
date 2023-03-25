#ifndef GLOBALDESIGNCONFIG_H
#define GLOBALDESIGNCONFIG_H

#include <configmodels/configinterface.h>

class GlobalDesignConfig : public ConfigInterface
{
    Q_OBJECT
    YACAPPPROPERTY(int, defaultFontPointSize, DefaultFontPointSize, 13);
public:
    explicit GlobalDesignConfig(QObject *parent = nullptr);

    void setConfig(const QJsonValue &config) override;
    QJsonObject getConfig() override;
};

#endif // GLOBALDESIGNCONFIG_H
