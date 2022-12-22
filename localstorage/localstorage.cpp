#include "localstorage.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>


LocalStorage::LocalStorage(Constants &constants):
    db(QSqlDatabase::addDatabase("QSQLITE"))
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

int LocalStorage::loadKnownContacts(AppendFunction appendFunction)
{
    QString sql("select * from ");
    sql += tableNames.knowncontacts;
    sql += " order by unread_messages desc ";
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

void LocalStorage::upsertKnownContact(ProfileObject *po)
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
    q.bindValue(":id", po->id());
    q.bindValue(":visible_name", po->visibleName());
    q.bindValue(":unread_messages", po->unreadMessages());
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
}
