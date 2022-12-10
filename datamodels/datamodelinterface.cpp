#include "datamodelinterface.h"
#include <QQmlContext>

template <class T>
bool DataModelInterface<T>::first(true);

template<class T>
QHash<int, QByteArray> DataModelInterface<T>::customRoleNames() const
{
    return QHash<int, QByteArray>();
}

template<class T>
QVariant DataModelInterface<T>::customData(int row, int role) const
{
    return QVariant();
}

template <class T>
DataModelInterface<T>::DataModelInterface(QQmlApplicationEngine &engine,
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

template <class T>
QModelIndex DataModelInterface<T>::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

template <class T>
QModelIndex DataModelInterface<T>::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

template <class T>
int DataModelInterface<T>::rowCount(const QModelIndex &parent) const
{
    return static_cast<int>(size());
}

template <class T>
int DataModelInterface<T>::columnCount(const QModelIndex &parent) const
{
    return 1;
}

template <class T>
QVariant DataModelInterface<T>::data(const QModelIndex &index, int role) const
{
    int row(direction == reverse ? size() - 1 - index.row() : index.row());
    if (row >= static_cast<int>(size()) || row < 0)
    {
        return QVariant();
    }
    QVariant ret(customData(row, role));
    if (ret != QVariant())
    {
        return ret;
    }
    if (role != ObjectRole && role != DefaultObjectRole)
    {
        return QVariant();
    }
    return QVariant::fromValue<QObject *>(getObject(static_cast<size_t>(row)));

}

template <class T>
QHash<int, QByteArray> DataModelInterface<T>::roleNames() const
{
    QHash<int, QByteArray> roles(customRoleNames());
    if (objectName.length())
    {
        roles[ObjectRole] = objectName.toLatin1();
    }
    roles[DefaultObjectRole] = "dataObject";
    return roles;
}

template <class T>
void DataModelInterface<T>::append(T *object)
{
    if (direction == reverse)
    {
        beginInsertRows(QModelIndex(), 0, 0);
    }
    else
    {
        beginInsertRows(QModelIndex(), size(), size());
    }
    internalAppend(object);
    endInsertRows();
}
