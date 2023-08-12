#include "logger.h"
#include <QDebug>

Logger::Logger(QObject *parent)
    : QObject{parent}
{

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



void Logger::log(QString file, int line, QSet<Category> categories, QString message)
{
    if (!message.size())
    {
        return;
    }
    if (categories.find(OnlyDesktop) != categories.end() && !isDesktop)
    {
        return;
    }
    QString lineString(QString::number(line));
    lineString = QString(" ").repeated(5 - lineString.size()) + lineString;
    file = file.right(fileChars);
    file = QString(" ").repeated(fileChars - file.size()) + file;
    qDebug() << file + ":" + lineString + ": " + message;
}
