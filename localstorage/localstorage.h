#ifndef LOCALSTORAGE_H
#define LOCALSTORAGE_H

#include <QSqlDatabase>
#include "constants.h"
#include "tablenames.h"
#include "dataobjects/dataobjectinterface.h"
#include "dataobjects/profileobject.h"
#include "dataobjects/messageobject.h"


class LocalStorage
{
    QSqlDatabase db;
    TableNames tableNames;
    bool tableHasColumn(const QString &tableName,
                        const QString &columnName);
    bool tableExists(const QString &tableName);
    void createTables();

    const QString insertMessageString;
public:
    LocalStorage(Constants &constants);

    void exec(const QString &sql);
    typedef std::function<void(DataObjectInterface *)> AppendFunction;
    int loadKnownContacts(AppendFunction appendFunction);
    void upsertKnownContact(const ProfileObject &po);

    int loadMessages(const QString &contactId, AppendFunction appendFunction);
    void insertMessage(const MessageObject &mo);
};

#endif // LOCALSTORAGE_H
