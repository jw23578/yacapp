#include "yacapp.h"
#include <QFileInfo>
#include <QStandardPaths>
#include <QJsonObject>

YACAPP::YACAPP(QObject *parent)
    : QObject{parent}
{
#ifdef Q_OS_WIN
    setIsDesktop(true);
#endif

#ifdef Q_OS_LINUX
    setIsDesktop(true);
#endif

    QStringList paths(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
    if (paths.size() == 0)
    {
        return;
    }
    stateFilename = paths[0] + "/yacAppState.json";
    qDebug() << stateFilename;
    QFile jsonFile(stateFilename);
    jsonFile.open(QIODevice::ReadOnly);
    QByteArray fileData(jsonFile.readAll());
    QJsonDocument config(QJsonDocument::fromJson(fileData));
    stringFromJSON(loginToken, LoginToken)
}

void YACAPP::init(QString projectFilename)
{
    projectFilename.replace("file://", "");
    this->projectFilename = projectFilename;
    QString rawFolder(QFileInfo(projectFilename).path());
    rawFolder.replace("file://", "");
    if (rawFolder.right(1) != '/')
    {
        rawFolder += "/";
    }
    setBaseUrl(rawFolder);
    if (baseUrl() == "")
    {
        return;
    }
    globalConfig()->init(projectFilename);
    for (int i(0); i < globalConfig()->formFiles.size(); ++i)
    {
        getConfig(globalConfig()->formFiles[i]);
    }
    for (int i(0); i < globalConfig()->menueFiles.size(); ++i)
    {
        getMenueConfig(globalConfig()->menueFiles[i]);
    }
    setMainConfig(getConfig(globalConfig()->mainFormFilename()));
}

void YACAPP::saveState()
{
    QJsonObject config;
    stringToJSON(loginToken);
    QFile jsonFile(stateFilename);
    jsonFile.open(QIODevice::WriteOnly);
    jsonFile.write(QJsonDocument(config).toJson());

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

void YACAPP::addKnownMenueFile(const QString &filename)
{
    if (m_knownMenueFiles.contains(filename))
    {
        return;
    }
    m_knownMenueFiles.append(filename);
    emit knownMenueFilesChanged();
}

void YACAPP::reset()
{
    {
        QMap<QString, ParsedConfig*>::iterator it(fileName2ParsedConfig.begin());
        while (it != fileName2ParsedConfig.end())
        {
            delete it.value();
            ++it;
        }
    }
    {
        QMap<QString, MenueConfig*>::iterator it(fileName2MenueConfig.begin());
        while (it != fileName2MenueConfig.end())
        {
            delete it.value();
            ++it;
        }
    }
    fileName2ParsedConfig.clear();
    fileName2MenueConfig.clear();
    delete m_globalConfig;
    m_globalConfig = new GlobalProjectConfig;
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

MenueConfig *YACAPP::getMenueConfig(const QString &filename)
{
    if (!filename.size())
    {
        return &emptyMenue;
    }
    QString fullFilename(baseUrl() + filename);
    QMap<QString, MenueConfig*>::Iterator configIt(fileName2MenueConfig.find(fullFilename));
    if (configIt == fileName2MenueConfig.end())
    {
        fileName2MenueConfig[fullFilename] = new MenueConfig;
        configIt = fileName2MenueConfig.find(fullFilename);
        configIt.value()->init(fullFilename);
    }
    addKnownMenueFile(filename);
    return configIt.value();
}

void YACAPP::loadNewProject(const QString &projectFilename)
{
    reset();
    init(projectFilename);
}

void YACAPP::saveCurrentProject()
{
    globalConfig()->save(projectFilename);
    {
        QMap<QString, ParsedConfig*>::iterator it(fileName2ParsedConfig.begin());
        while (it != fileName2ParsedConfig.end())
        {
            it.value()->save(it.key(), baseUrl());
            ++it;
        }
    }
    {
        QMap<QString, MenueConfig*>::iterator it(fileName2MenueConfig.begin());
        while (it != fileName2MenueConfig.end())
        {
            it.value()->save(it.key());
            ++it;
        }
    }
}

