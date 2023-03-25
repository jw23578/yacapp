#ifndef MULTIPURPOSEOBJECT_H
#define MULTIPURPOSEOBJECT_H

#include "dataobjectinterface.h"

class MultiPurposeObject : public DataObjectInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QString, name, Name, "");
    YACAPPPROPERTY(QString, caption, Caption, "");
public:
    explicit MultiPurposeObject(QObject *parent = nullptr);

    void assign(const MultiPurposeObject &other);
};

#endif // MULTIPURPOSEOBJECT_H
