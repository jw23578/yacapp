#ifndef LOCALSTORAGE_H
#define LOCALSTORAGE_H

#include <QSqlDatabase>
#include "constants.h"
#include "tablenames.h"
#include "dataobjects/dataobjectinterface.h"
#include "datamodels/datamodelinterface.h"


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

    int loadKnownContacts(DataModelInterface)
};

#endif // LOCALSTORAGE_H
