#include "logger.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Logger::Logger(QObject *parent)
    : QObject{parent}
{

}

QString Logger::categoryToString(const Category c)
{
    switch (c)
    {
    case Default: return "Default";
    case OnlyDesktop: return "OnlyDesktop";
    case Verbose: return "Verbose";
    case Fatal: return "Fatal";
    case Javascript: return "Javascript";
    default: return "";
    }
}

void Logger::logToFile(const QString &file, int line, QSet<Category> categories, const QString &message)
{
    if (!logfile.isOpen())
    {
        return;
    }
    QJsonArray cats;
    for (const auto &c: categories)
    {
        cats.append(categoryToString(c));
    }
    QJsonObject json;
    json["categories"] = cats;
    json["file"] = file;
    json["line"] = line;
    json["message"] = message;
    json["msecsSinceEPoch"] = QDateTime::currentMSecsSinceEpoch();
    QJsonDocument doc(json);
    logfile.write(doc.toJson(QJsonDocument::Compact) + "\r\n");
    logfile.flush();
}

Logger &Logger::gi()
{
    if (!theInstance)
    {
        theInstance = new Logger;
    }
    return *theInstance;
}

Logger *Logger::theInstance(0);

void Logger::setIsDesktop(bool v)
{
    isDesktop = v;
}

void Logger::setLogFile(const QString &filePathAndName)
{
    logfile.setFileName(filePathAndName);
    logfile.open(QIODevice::WriteOnly);
}



void Logger::log(QString file, int line, QSet<Category> categories, const QString &message)
{
    if (!message.size())
    {
        return;
    }
    if (categories.find(OnlyDesktop) != categories.end() && !isDesktop)
    {
        return;
    }
    logToFile(file, line, categories, message);
    QString lineString(QString::number(line));
    lineString = QString(" ").repeated(5 - lineString.size()) + lineString;
    file = file.right(fileChars);
    file = QString(" ").repeated(fileChars - file.size()) + file;
    QString jsLog(categories.contains(Javascript) ? "jsLog: " : "");
    qDebug() << file + ":" + lineString + ": " + jsLog + message;
}
