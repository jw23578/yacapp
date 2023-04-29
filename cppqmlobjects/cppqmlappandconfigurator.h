#ifndef CPPQMLAPPANDCONFIGURATOR_H
#define CPPQMLAPPANDCONFIGURATOR_H

#include <QObject>
#include <QJSValue>

class CPPQMLAppAndConfigurator : public QObject
{
    Q_OBJECT
public:
    explicit CPPQMLAppAndConfigurator(QObject *parent = nullptr);


signals:
    void yesNoQuestion(const QString &question, QJSValue itemToFocus, QJSValue yesCallback, QJSValue noCallback);
    void badMessage(const QString &message, QJSValue itemToFocus, QJSValue okCallback);
    void goodMessage(const QString &message, QJSValue itemToFocus, QJSValue okCallback);

};

#endif // CPPQMLAPPANDCONFIGURATOR_H
