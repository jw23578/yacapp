#ifndef LOCALSTORAGE_H
#define LOCALSTORAGE_H

#include <QSqlDatabase>
#include "constants.h"
#include "localtablenames.h"
#include "dataobjects/dataobjectinterface.h"
#include "dataobjects/profileobject.h"
#include "dataobjects/messageobject.h"
#include <QSqlQuery>


class LocalStorage
{
    QSqlDatabase db;
    LocalTableNames tableNames;
    TableFields tableFields;
    bool tableHasColumn(const QString &tableName,
                        const QString &columnName);
    bool renameColumn(const QString &tableName,
                      const QString &oldColumnName,
                      const QString &newColumnName);
    bool renameColumnIfNeeded(const QString &tableName,
                              const QString &oldColumnName,
                              const QString &newColumnName);
    bool addColumnIfNeeded(const QString &tableName,
                           const QString &columnName,
                           const QString &columnType);
    bool tableExists(const QString &tableName);
    bool createExec(const QString &sql);
    void createTables();

    const QString selectOneMessageString;
    const QString insertMessageString;
    const QString updateMessageString;
    const QString deleteMessageString;
    const QString deleteAllMessagesString;
    const QString deleteKnownContactString;

    void bindQDateTimeToMsecs(QSqlQuery &q, QString const column, QDateTime const dt) const;
    QDateTime getDateTimeFromMSecs(QSqlQuery &q, int column) const;

    bool updateMessage(const MessageObject &mo) const;
public:
    LocalStorage(QString appId, Constants &constants);
    ~LocalStorage();

    bool exec(QSqlQuery &q) const;
    bool exec(const QString &sql) const;
    typedef std::function<void(DataObjectInterface *)> AppendFunction;
    int loadKnownContacts(AppendFunction appendFunction);
    void upsertKnownContact(const ProfileObject &po);
    void deleteKnownContact(const QString &id);

    bool messageExists(const QString &id) const;
    int loadMessages(const QString &contactId, AppendFunction appendFunction);
    bool upsertMessage(const MessageObject &mo, bool &inserted, bool &updated) const;
    void deleteMessage(const QString &id);
    void deleteAllMessages();
};

#endif // LOCALSTORAGE_H
