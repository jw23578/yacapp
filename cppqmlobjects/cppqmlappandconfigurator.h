#ifndef CPPQMLAPPANDCONFIGURATOR_H
#define CPPQMLAPPANDCONFIGURATOR_H

#include <QObject>
#include <QJSValue>
#include <QDateTime>

class CPPQMLAppAndConfigurator : public QObject
{
    Q_OBJECT
public:
    explicit CPPQMLAppAndConfigurator(QObject *parent = nullptr);


signals:
    void waitMessage(const QString &message);
    void hideWaitMessage();

    void yesNoQuestion(const QString &question, QJSValue itemToFocus, QJSValue yesCallback, QJSValue noCallback);
    void badMessage(const QString &message, QJSValue itemToFocus, QJSValue okCallback);
    void goodMessage(const QString &message, QJSValue itemToFocus, QJSValue okCallback);

    void pickDateTime(const QDateTime dateTime, const QJSValue okCallback);
    void pickTime(const QDateTime time, const QJSValue okCallback);
    void pickDate(const QDateTime date, const QJSValue okCallback);

    void notYetImplemented();
};

#endif // CPPQMLAPPANDCONFIGURATOR_H
