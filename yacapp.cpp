#include "yacapp.h"

YACAPP::YACAPP(QObject *parent)
    : QObject{parent}
{
}

void YACAPP::init()
{
    if (baseUrl() == "")
    {
        return;
    }
    globalConfig()->init(baseUrl() + "global.json");
    mainConfig()->init(baseUrl() + globalConfig()->mainFormFilename(),
                       baseUrl());
    m_knownFiles.clear();
    fileName2ParsedConfig[baseUrl() + globalConfig()->mainFormFilename()] = mainConfig();
    addKnownFile(globalConfig()->mainFormFilename());
}

void YACAPP::addKnownFile(QString const &filename)
{
    if (m_knownFiles.contains(filename))
    {
        return;
    }
    m_knownFiles.append(filename);
    emit knownFilesChanged();
}

ParsedConfig *YACAPP::getConfig(const QString &filename)
{
    QString fullFilename(baseUrl() + filename);
    QMap<QString, ParsedConfig*>::Iterator configIt(fileName2ParsedConfig.find(fullFilename));
    if (configIt == fileName2ParsedConfig.end())
    {
        fileName2ParsedConfig[fullFilename] = new ParsedConfig;
        configIt = fileName2ParsedConfig.find(fullFilename);
        configIt.value()->init(fullFilename, baseUrl());
    }
    addKnownFile(filename);
    return configIt.value();
}

void YACAPP::loadNewProject(const QString &folder)
{
    QString rawFolder(folder);
    rawFolder.replace("file://", "");
    if (rawFolder.right(1) != '/')
    {
        rawFolder += "/";
    }
    setBaseUrl(rawFolder);
    init();
}

void YACAPP::saveCurrentProject()
{
    QMap<QString, ParsedConfig*>::iterator it(fileName2ParsedConfig.begin());
    while (it != fileName2ParsedConfig.end())
    {
      it.value()->save(it.key(), baseUrl());
      ++it;
    }
}

