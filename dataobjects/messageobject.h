#ifndef MESSAGEOBJECT_H
#define MESSAGEOBJECT_H

#include "dataobjectinterface.h"

class MessageObject : public DataObjectInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QString, content, Content, "");
public:
    explicit MessageObject(const QString &content);

signals:

};

#endif // MESSAGEOBJECT_H
