#ifndef LOCALSTORAGE_H
#define LOCALSTORAGE_H

#include <QSqlDatabase>
#include "constants.h"
#include "localtablenames.h"
#include "dataobjects/dataobjectinterface.h"
#include "dataobjects/profileobject.h"
#include "dataobjects/messageobject.h"


class LocalStorage
{
    QSqlDatabase db;
    LocalTableNames tableNames;
    bool tableHasColumn(const QString &tableName,
                        const QString &columnName);
    bool tableExists(const QString &tableName);
    void createTables();

    const QString selectOneMessageString;
    const QString insertMessageString;
    const QString deleteMessageString;
    const QString deleteKnownContactString;
public:
    LocalStorage(QString appId, Constants &constants);

    void exec(const QString &sql);
    typedef std::function<void(DataObjectInterface *)> AppendFunction;
    int loadKnownContacts(AppendFunction appendFunction);
    void upsertKnownContact(const ProfileObject &po);
    void deleteKnownContact(const QString &id);

    bool messageExists(const QString &id);
    int loadMessages(const QString &contactId, AppendFunction appendFunction);
    bool insertMessage(const MessageObject &mo);
    void deleteMessage(const QString &id);
};

#endif // LOCALSTORAGE_H
