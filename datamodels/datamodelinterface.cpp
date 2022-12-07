#include "datamodelinterface.h"
#include <QQmlContext>

bool DataModelInterface::first(true);

DataModelInterface::DataModelInterface(QQmlApplicationEngine &engine,
                                       const QString &modelName,
                                       const QString &objectName,
                                       const DirectionType direction)
    : QAbstractItemModel{&engine},
      modelName(modelName),
      objectName(objectName),
      direction(direction)
{
    if (first)
    {
        first = false;
        qmlRegisterType<DataObjectInterface>("DataObjectInterface", 1, 0, "DataObjectInterface");
    }
    engine.rootContext()->setContextProperty(modelName, QVariant::fromValue(this));
}

QModelIndex DataModelInterface::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex DataModelInterface::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int DataModelInterface::rowCount(const QModelIndex &parent) const
{
    return static_cast<int>(objects.size());
}

int DataModelInterface::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant DataModelInterface::data(const QModelIndex &index, int role) const
{
    int row(direction == reverse ? objects.size() - 1 - index.row() : index.row());
    if (row >= static_cast<int>(objects.size()) || row < 0)
    {
        return QVariant();
    }
    if (role != ObjectRole && role != DefaultObjectRole)
    {
        return QVariant();
    }
    return QVariant::fromValue<QObject *>(objects[static_cast<size_t>(row)]);

}

QHash<int, QByteArray> DataModelInterface::roleNames() const
{
    QHash<int, QByteArray> roles;
    if (objectName.length())
    {
        roles[ObjectRole] = objectName.toLatin1();
    }
    roles[DefaultObjectRole] = "dataObject";
    return roles;
}

void DataModelInterface::append(DataObjectInterface *object)
{
    if (direction == reverse)
    {
        beginInsertRows(QModelIndex(), 0, 0);
    }
    else
    {
        beginInsertRows(QModelIndex(), objects.size(), objects.size());
    }
    objects.push_back(object);
    endInsertRows();
}
