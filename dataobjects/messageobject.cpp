#include "messageobject.h".h"

MessageObject::MessageObject(const QString &content)
    : DataObjectInterface{0}
{
    setContent(content);
}
