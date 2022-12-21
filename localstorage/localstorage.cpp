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

bool LocalStorage::tableHasColumn(const QString &tableName,
                    const QString &columnName)
{
    QString sql("select * from ");
    sql += tableName;
    sql += " limit 1";
    QSqlQuery q(sql);
    if (!q.exec())
    {
        return false;
    }
    const QSqlRecord &record(q.record());
    return record.indexOf(columnName) >= 0;
}


bool LocalStorage::tableExists(const QString &tableName)
{
    QSqlQuery q("select name from sqlite_master "
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
        q.exec();
    }
}
