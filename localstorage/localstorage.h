#ifndef LOCALSTORAGE_H
#define LOCALSTORAGE_H

#include <QSqlDatabase>
#include "constants.h"
#include "tablenames.h"
#include "dataobjects/dataobjectinterface.h"
#include "dataobjects/profileobject.h"


class LocalStorage
{
    QSqlDatabase db;
    TableNames tableNames;
    bool tableHasColumn(const QString &tableName,
                        const QString &columnName);
    bool tableExists(const QString &tableName);
    void createTables();
public:
    LocalStorage(Constants &constants);

    typedef std::function<void(DataObjectInterface *)> AppendFunction;
    int loadKnownContacts(AppendFunction appendFunction);
    void upsertKnownContact(ProfileObject *po);
};

#endif // LOCALSTORAGE_H
