#include "localstorage.h"
#include <QSqlError>
#include <QSqlRecord>
#include "logger.h"
#include <QVariant>

LocalStorage::LocalStorage(QString appId,
                           Constants &constants):
    db(QSqlDatabase::addDatabase("QSQLITE")),
    selectOneMessageString("select * from messages "
                           "where id = :id "
                           "limit 1"),
    insertMessageString("insert into messages (id, sender_id, receiver_or_group_id, content, sent_msecs, received_msecs, read, deleted_msecs) "
                        " values "
                        "(:id, :sender_id, :receiver_or_group_id, :content, :sent_msecs, :received_msecs, :read, :deleted_msecs)"),
    deleteMessageString(QString("update ") + tableNames.messages
                        + QString(" set deleted_msecs = :deleted_msecs where id = :id")),
    deleteAllMessagesString(QString("update ") + tableNames.messages
                              + QString(" set deleted_msecs = :deleted_msecs where deleted_msecs is null")),
    deleteKnownContactString(QString("delete from ") + tableNames.knowncontacts
                             + QString(" where id = :id"))
{
    db.setDatabaseName(constants.getDBFilename(appId));
    DEFAULT_LOG(QString("dbFilename: ") + constants.getDBFilename(appId));
    if (db.open())
    {
        DEFAULT_LOG("db opened successfully");
        createTables();
    }
    else
    {
        DEFAULT_LOG("db not opened");
        DEFAULT_LOG(db.lastError().databaseText());
    }

}

LocalStorage::~LocalStorage()
{
    db.close();
}

bool LocalStorage::exec(QSqlQuery &q)
{
    if (!q.exec())
    {
        FATAL_LOG(QString("Error executing Query: ") + q.lastError().text());
        FATAL_LOG(QString("Query was: ") + q.lastQuery());
        return false;
    }
    return true;
}


bool LocalStorage::exec(const QString &sql)
{
    QSqlQuery q;
    q.prepare(sql);
    return exec(q);
}

int LocalStorage::loadKnownContacts(AppendFunction appendFunction)
{
    QString sql("select * from ");
    sql += tableNames.knowncontacts;
    sql += " order by unread_messages desc,"
           " visible_name ";
    QSqlQuery q(sql);
    int idColumn(q.record().indexOf("id"));
    int visibleNameColumn(q.record().indexOf("visible_name"));
    int unreadMessagesColumn(q.record().indexOf("unread_messages"));
    int imageIdColumn(q.record().indexOf("image_id"));
    int public_key_base64Column(q.record().indexOf(tableFields.public_key_base64));
    int count(0);
    while (q.next())
    {
        ProfileObject *po(new ProfileObject);
        po->setId(q.value(idColumn).toString());
        po->setVisibleName(q.value(visibleNameColumn).toString());
        po->setUnreadMessages(q.value(unreadMessagesColumn).toInt());
        po->setProfileImageId(q.value(imageIdColumn).toString());
        po->setPublic_key_base64(q.value(public_key_base64Column).toString());
        appendFunction(po);
        count += 1;
    }
    return count;
}

void LocalStorage::upsertKnownContact(const ProfileObject &po)
{
    QString sql("insert into ");
    sql += tableNames.knowncontacts;
    sql += QString(" (id, visible_name, unread_messages, image_id, public_key_base64) ");
    sql += QString(" values ");
    sql += QString(" (:id, :visible_name, :unread_messages, :image_id, :public_key_base64) ");
    sql += QString(" on conflict(id) do update set ");
    sql += QString(" visible_name = :visible_name, unread_messages = :unread_messages, image_id = :image_id, public_key_base64 = :public_key_base64 ");
    sql += QString(" where id = :id ");
    QSqlQuery q;
    q.prepare(sql);
    q.bindValue(":id", po.id());
    q.bindValue(":visible_name", po.visibleName());
    q.bindValue(":unread_messages", po.unreadMessages());
    q.bindValue(":image_id", po.profileImageId());
    q.bindValue(":public_key_base64", po.public_key_base64());
    exec(q);
}

void LocalStorage::deleteKnownContact(const QString &id)
{
    QSqlQuery q;
    q.prepare(deleteKnownContactString);
    q.bindValue(":id", id);
    exec(q);
}

bool LocalStorage::messageExists(const QString &id)
{
    QSqlQuery q;
    q.prepare(selectOneMessageString);
    q.bindValue(":id", id);
    exec(q);
    return q.first();
}

int LocalStorage::loadMessages(const QString &contactId,
                               AppendFunction appendFunction)
{
    QString sql("select * from ");
    sql += tableNames.messages;
    sql += QString(" where (sender_id = :contactId ");
    sql += QString(" or receiver_or_group_id = :contactId) ");
    sql += QString(" and deleted_msecs is null ");
    sql += QString(" order by received_msecs ");
    QSqlQuery q;
    q.prepare(sql);
    q.bindValue(":contactId", contactId);
    exec(q);
    int idColumn(q.record().indexOf("id"));
    int sender_idColumn(q.record().indexOf("sender_id"));
    int receiver_or_group_idColumn(q.record().indexOf("receiver_or_group_id"));
    int contentColumn(q.record().indexOf("content"));
    int sent_msecsColumn(q.record().indexOf("sent_msecs"));
    int received_msecsColumn(q.record().indexOf("received_msecs"));
    int readColumn(q.record().indexOf("read"));
    int deleted_msecsColumn(q.record().indexOf("deleted_datetime_msecs"));
    int count(0);
    while (q.next())
    {
        QDateTime sent(QDateTime::fromMSecsSinceEpoch(q.value(sent_msecsColumn).toLongLong()));
        QDateTime received(QDateTime::fromMSecsSinceEpoch(q.value(received_msecsColumn).toLongLong()));
        QDateTime deleted_datetime(getDateTimeFromMSecs(q, deleted_msecsColumn));
        MessageObject *mo(new MessageObject(q.value(idColumn).toString(),
                                            q.value(sender_idColumn).toString(),
                                            q.value(receiver_or_group_idColumn).toString(),
                                            sent,
                                            received,
                                            deleted_datetime,
                                            q.value(contentColumn).toString(),
                                            q.value(readColumn).toBool(),
                                            ""));
        appendFunction(mo);
        ++count;
    }
    return count;
}

bool LocalStorage::insertMessage(const MessageObject &mo)
{
    if (messageExists(mo.id()))
    {
        return false;
    }
    QSqlQuery q;
    q.prepare(insertMessageString);
    q.bindValue(":id", mo.id());
    q.bindValue(":sender_id", mo.senderId());
    q.bindValue(":receiver_or_group_id", mo.receiverId());
    q.bindValue(":content", mo.content());
    bindQDateTimeToMsecs(q, ":sent_msecs", mo.sent());
    bindQDateTimeToMsecs(q, ":received_msecs", mo.received());
    q.bindValue(":read", mo.read());
    bindQDateTimeToMsecs(q, ":deleted_msecs", mo.deleted_datetime());
    exec(q);
    return true;
}

void LocalStorage::deleteMessage(const QString &id)
{
    QSqlQuery q;
    q.prepare(deleteMessageString);
    q.bindValue(":id", id);
    bindQDateTimeToMsecs(q, ":deleted_msecs", QDateTime::currentDateTime());
    exec(q);
}

void LocalStorage::deleteAllMessages()
{
    QSqlQuery q;
    q.prepare(deleteAllMessagesString);
    bindQDateTimeToMsecs(q, ":deleted_msecs", QDateTime::currentDateTime());
    exec(q);
}

bool LocalStorage::tableHasColumn(const QString &tableName,
                                  const QString &columnName)
{
    QString sql("select * from ");
    sql += tableName;
    sql += " limit 1";
    QSqlQuery q;
    q.prepare(sql);
    if (!exec(q))
    {
        return false;
    }
    const QSqlRecord &record(q.record());
    return record.indexOf(columnName) >= 0;
}

bool LocalStorage::renameColumn(const QString &tableName,
                                const QString &oldColumnName,
                                const QString &newColumnName)
{
    QString sql("alter table ");
    sql += tableName;
    sql += QString(" rename ");
    sql += oldColumnName;
    sql += QString(" to ");
    sql += newColumnName;
    return exec(sql);
}

bool LocalStorage::renameColumnIfNeeded(const QString &tableName,
                                        const QString &oldColumnName,
                                        const QString &newColumnName)
{
    if (tableHasColumn(tableName, newColumnName))
    {
        return true;
    }
    return renameColumn(tableName, oldColumnName, newColumnName);
}

bool LocalStorage::addColumnIfNeeded(const QString &tableName,
                                     const QString &columnName,
                                     const QString &columnType)
{
    if (tableHasColumn(tableName,
                       columnName))
    {
        return true;
    }
    QString sql("alter table ");
    sql += tableName;
    sql += QString(" add column ");
    sql += columnName + " " + columnType;
    return exec(sql);
}


bool LocalStorage::tableExists(const QString &tableName)
{
    QSqlQuery q;
    q.prepare("select name from sqlite_master "
              "where type = 'table' and name = :tableName "
              "limit 1");
    q.bindValue(":tableName", tableName);
    exec(q);
    return q.next();
}

bool LocalStorage::createExec(const QString &sql)
{
    QSqlQuery q;
    if (!q.exec(sql))
    {
        FATAL_LOG(QString("Error executing Query: ") + q.lastError().text());
        FATAL_LOG(QString("Query was: ") + q.lastQuery());
        return false;
    }
    return true;
}

void LocalStorage::createTables()
{
    if (!tableExists(tableNames.knowncontacts))
    {
        QString q("create table knowncontacts ( "
                  "id text primary key, "
                  "visible_name text, "
                  "unread_messages int) ");
        createExec(q);
    }
    addColumnIfNeeded(tableNames.knowncontacts, tableFields.image_id, "text");
    addColumnIfNeeded(tableNames.knowncontacts, tableFields.public_key_base64, "text");
    if (!tableExists(tableNames.messages))
    {
        QString sql("create table ");
        sql += tableNames.messages;
        sql += QString("( id text primary key ");
        sql += QString(" , sender_id text ");
        sql += QString(" , receiver_or_group_id text ");
        sql += QString(" , content text ");
        sql += QString(" , sent_msecs int8 ");
        sql += QString(" , received_msecs int8 ");
        sql += QString(" , read bool) ");
        createExec(sql);
        createExec("create index message_i1 on messages (sender_id)");
        createExec("create index message_i2 on messages (receiver_or_group_id)");
    }
    renameColumnIfNeeded(tableNames.messages, "deleted_datetime_msecs", "deleted_msecs");
    addColumnIfNeeded(tableNames.messages, "deleted_msecs", "int8");
}

void LocalStorage::bindQDateTimeToMsecs(QSqlQuery &q, const QString column, const QDateTime dt) const
{
    if (dt.isNull())
    {
        q.bindValue(column, QDateTime());
    }
    else
    {
        q.bindValue(column, dt.toMSecsSinceEpoch());
    }
}

QDateTime LocalStorage::getDateTimeFromMSecs(QSqlQuery &q, int column) const
{
    qlonglong msecs(q.value(column).toLongLong());
    if (msecs <= 0)
    {
        return QDateTime();
    }
    return QDateTime::fromMSecsSinceEpoch(msecs);
}
