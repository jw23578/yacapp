#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QSet>

class Logger : public QObject
{    
    Q_OBJECT
    const int fileChars = 50;
    explicit Logger(QObject *parent = nullptr);
    static Logger *theInstance;
    bool isDesktop = false;
public:
    static Logger &gi();
    enum Category
    {
        Default,
        OnlyDesktop,
        Verbose,
        Fatal
    };

    void log(QString file, int line, QSet<Category> categories, QString message);
    void setIsDesktop(bool v);

signals:

};

#define DEFAULT_LOG(message) Logger::gi().log(__FILE__, __LINE__, {Logger::Default}, message)
#define DEFAULT_LOG_VARIABLE(variable) Logger::gi().log(__FILE__, __LINE__, {Logger::Default}, QString(#variable) + ": " + variable);
#define ONLY_DESKTOP_LOG(message) Logger::gi().log(__FILE__, __LINE__, {Logger::OnlyDesktop}, message)

#endif // LOGGER_H
