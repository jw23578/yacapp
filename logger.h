#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QSet>
#include <QFile>

class Logger : public QObject
{
    Q_OBJECT
public:
    enum Category
    {
        Default,
        OnlyDesktop,
        Verbose,
        Fatal,
        Javascript
    };
private:
    const int fileChars = 50;
    explicit Logger(QObject *parent = nullptr);
    static Logger *theInstance;
    bool isDesktop = false;
    QFile logfile;
    QString categoryToString(const Category c);
    void logToFile(const QString &file, int line, QSet<Category> categories, const QString &message);
public:
    static Logger &gi();

    void log(QString file, int line, QSet<Category> categories, const QString &message);
    void setIsDesktop(bool v);
    void setLogFile(const QString &filePathAndName);

signals:

};

#define JAVASCRIPT_LOG(message) Logger::gi().log(__FILE__, __LINE__, {Logger::Javascript}, message)
#define DEFAULT_LOG(message) Logger::gi().log(__FILE__, __LINE__, {Logger::Default}, message)
#define DEFAULT_LOG_VARIABLE(variable) Logger::gi().log(__FILE__, __LINE__, {Logger::Default}, QString(#variable) + ": " + variable);
#define ONLY_DESKTOP_LOG(message) Logger::gi().log(__FILE__, __LINE__, {Logger::OnlyDesktop}, message)

#endif // LOGGER_H
