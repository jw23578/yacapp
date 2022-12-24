#include "localstorage.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

LocalStorage::LocalStorage(Constants &constants):
    db(QSqlDatabase::addDatabase("QSQLITE")),
    insertMessageString("insert into messages (id, sender_id, receiver_or_group_id, content, sent_msecs, received_msecs, read) "
                        " values "
                        "(:id, :sender_id, :receiver_or_group_id, :content, :sent_msecs, :received_msecs, :read)")
{
    db.setDatabaseName(constants.getDBFilename());
    qDebug() << "dbFilename: " << constants.getDBFilename();
    if (db.open())
    {
        qDebug() << "db opened successfully";
        createTables();
    }
    else
    {
        qDebug() << "db not opened";
        qDebug() << db.lastError().databaseText();
    }

}

void LocalStorage::exec(const QString &sql)
{
    QSqlQuery q(sql);
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
    while (q.next())
    {
        ProfileObject *po(new ProfileObject);
        po->setId(q.value(idColumn).toString());
        po->setVisibleName(q.value(visibleNameColumn).toString());
        po->setUnreadMessages(q.value(unreadMessagesColumn).toInt());
        appendFunction(po);
    }
}

void LocalStorage::upsertKnownContact(const ProfileObject &po)
{
    QString sql("insert into ");
    sql += tableNames.knowncontacts;
    sql += QString(" (id, visible_name, unread_messages) ");
    sql += QString(" values ");
    sql += QString(" (:id, :visible_name, :unread_messages) ");
    sql += QString(" on conflict(id) do update set ");
    sql += QString(" visible_name = :visible_name, unread_messages = :unread_messages ");
    sql += QString(" where id = :id ");
    QSqlQuery q;
    q.prepare(sql);
    q.bindValue(":id", po.id());
    q.bindValue(":visible_name", po.visibleName());
    q.bindValue(":unread_messages", po.unreadMessages());
    q.exec();
}

int LocalStorage::loadMessages(const QString &contactId,
                               AppendFunction appendFunction)
{
    QString sql("select * from ");
    sql += tableNames.messages;
    sql += QString(" where sender_id = :contactId ");
    sql += QString(" or receiver_or_group_id = :contactId ");
    sql += QString(" order by received_msecs ");
    QSqlQuery q;
    q.prepare(sql);
    q.bindValue(":contactId", contactId);
    q.exec();
    int idColumn(q.record().indexOf("id"));
    int sender_idColumn(q.record().indexOf("sender_id"));
    int receiver_or_group_idColumn(q.record().indexOf("receiver_or_group_id"));
    int contentColumn(q.record().indexOf("content"));
    int sent_msecsColumn(q.record().indexOf("sent_msecs"));
    int received_msecsColumn(q.record().indexOf("received_msecs"));
    int readColumn(q.record().indexOf("read"));
    while (q.next())
    {
        QDateTime sent(QDateTime::fromMSecsSinceEpoch(q.value(sent_msecsColumn).toLongLong()));
        QDateTime received(QDateTime::fromMSecsSinceEpoch(q.value(received_msecsColumn).toLongLong()));
        MessageObject *mo(new MessageObject(q.value(idColumn).toString(),
                                            q.value(sender_idColumn).toString(),
                                            q.value(receiver_or_group_idColumn).toString(),
                                            sent,
                                            received,
                                            q.value(contentColumn).toString(),
                                            q.value(readColumn).toBool()));
        appendFunction(mo);
    }
}

void LocalStorage::insertMessage(const MessageObject &mo)
{
    QSqlQuery q;
    q.prepare(insertMessageString);
    q.bindValue(":id", mo.id());
    q.bindValue(":sender_id", mo.senderId());
    q.bindValue(":receiver_or_group_id", mo.receiverId());
    q.bindValue(":content", mo.content());
    q.bindValue(":sent_msecs", mo.sent().toMSecsSinceEpoch());
    q.bindValue(":received_msecs", mo.received().toMSecsSinceEpoch());
    q.bindValue(":read", mo.read());
    q.exec();
}

bool LocalStorage::tableHasColumn(const QString &tableName,
                                  const QString &columnName)
{
    QString sql("select * from ");
    sql += tableName;
    sql += " limit 1";
    QSqlQuery q;
    q.prepare(sql);
    if (!q.exec())
    {
        return false;
    }
    const QSqlRecord &record(q.record());
    return record.indexOf(columnName) >= 0;
}


bool LocalStorage::tableExists(const QString &tableName)
{
    QSqlQuery q;
    q.prepare("select name from sqlite_master "
              "where type='table' and name = :tableName "
              "limit 1");
    q.bindValue("tableName", tableName);
    q.exec();
    return q.size() > 0;
}

void LocalStorage::createTables()
{
    if (!tableExists(tableNames.knowncontacts))
    {
        QSqlQuery q("create table knowncontacts ( "
                    "id text primary key, "
                    "visible_name text, "
                    "unread_messages int) ");
    }
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
        exec(sql);
        exec("create index message_i1 on messages (sender_id)");
        exec("create index message_i2 on messages (receiver_or_group_id)");
    }
}
