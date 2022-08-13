#include "yacapp.h"

YACAPP::YACAPP(QObject *parent)
    : QObject{parent}
{
    mainConfig()->init("/home/jw78/wes23/main.json");
}

ParsedConfig *YACAPP::getConfig(const QString &fileName)
{
    QMap<QString, ParsedConfig*>::Iterator configIt(fileName2ParsedConfig.find(fileName));
    if (configIt == fileName2ParsedConfig.end())
    {
        fileName2ParsedConfig[fileName] = new ParsedConfig;
        configIt = fileName2ParsedConfig.find(fileName);
        configIt.value()->init(fileName);
    }
    return configIt.value();
}

