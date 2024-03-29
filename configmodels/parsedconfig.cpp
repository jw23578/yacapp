#include "parsedconfig.h"
#include <QFile>
#include <QQmlEngine>
#include <QJsonObject>

ParsedConfig::ParsedConfig(QObject *parent):
    QObject(parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

ParsedConfig::~ParsedConfig()
{
    delete m_background;
    delete m_menue;
    delete m_content;
    delete m_header;
    delete m_splashscreen;
}

bool ParsedConfig::init(const QString &jsonConfigFile)
{
    QFile jsonFile(jsonConfigFile);
    jsonFile.open(QIODevice::ReadOnly);
    if (!jsonFile.isOpen())
    {
        qDebug() << __FILE__ << ": " << __LINE__ << "fatal error could not open: " << jsonConfigFile;
        return false;
    }
    QByteArray fileData(jsonFile.readAll());
    QJsonDocument config = QJsonDocument::fromJson(fileData);

    stringFromJSON(menueFilename, MenueFilename);
    background()->setConfig(config["background"]);
    content()->setConfig(config["content"]);
    header()->setConfig(config["header"]);
    splashscreen()->setConfig(config["splashscreen"]);
    return true;
}

void ParsedConfig::save(const QString &jsonConfigFile)
{
    QJsonObject config;
    stringToJSON(menueFilename);
    config["background"] = background()->getConfig();
    config["header"] = header()->getConfig();
    config["content"] = content()->getConfig();
    config["splashscreen"] = splashscreen()->getConfig();

    QJsonObject::Iterator it(config.begin());
    while (it != config.end())
    {
        if (it.value() == QJsonObject())
        {
            it = config.erase(it);
        }
        else
        {
            ++it;
        }
    }

    QFile jsonFile(jsonConfigFile);
    jsonFile.open(QIODevice::WriteOnly);
    jsonFile.write(QJsonDocument(config).toJson());
}
