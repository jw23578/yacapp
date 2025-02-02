#include "datamodelinterface.h"
#include <QQmlContext>
#include "dataobjects/dataobjectinterface.h"

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
    Q_UNUSED(row);
    Q_UNUSED(role);
    return QVariant();
}

template <class T>
DataModelInterface<T>::DataModelInterface(QQmlApplicationEngine &engine,
                                          const QString &modelName,
                                          const QString &objectName,
                                          const DirectionType direction):
    DataModelQObjectInterface(),
    modelName(modelName),
    objectName(objectName),
    direction(direction)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    if (first)
    {
        first = false;
        qmlRegisterType<DataObjectInterface>("DataObjectInterface", 1, 0, "DataObjectInterface");
    }
    engine.rootContext()->setContextProperty(modelName, QVariant::fromValue(this));
}

template<class T>
DataModelInterface<T>::DataModelInterface(const QString &objectName,
                                          const DirectionType direction):
    DataModelQObjectInterface(),
    objectName(objectName),
    direction(direction)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

template<class T>
typename DataModelInterface<T>::DirectionType DataModelInterface<T>::getDirection() const
{
    return direction;
}

template <class T>
QModelIndex DataModelInterface<T>::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return createIndex(row, column);
}

template <class T>
QModelIndex DataModelInterface<T>::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

template <class T>
int DataModelInterface<T>::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(size());
}

template <class T>
int DataModelInterface<T>::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
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
    return QVariant::fromValue<QObject *>(internalGetObject(static_cast<size_t>(row)));

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
bool DataModelInterface<T>::append(T *object)
{
    if (!canAppend(object))
    {
        delete object;
        return false;
    }
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
    setCount(size());
    return true;
}

template<class T>
void DataModelInterface<T>::removeByIndex(const size_t index)
{
    internalRemoveByIndex(index);
}

template<class T>
void DataModelInterface<T>::clear()
{
    if (!size())
    {
        return;
    }
    beginResetModel();
    internalClear();
    endResetModel();
}

template<class T>
const T &DataModelInterface<T>::get(size_t index) const
{
    return *internalGetObject(index);
}

template<class T>
const T &DataModelInterface<T>::operator[](const size_t index) const
{
    return internalGetObject(index);
}

template<class T>
size_t DataModelInterface<T>::oneBubbleSort(bool withSwapUpdates, compareFunction cf)
{
    size_t swapped(0);
    for (size_t i(0); i < size() - 1; ++i)
    {
        T *a(internalGetObject(i));
        T *b(internalGetObject(i + 1));
        if (cf(*a, *b))
        {
            swap(i, i + 1, withSwapUpdates);
            ++swapped;
        }
    }
    return swapped;
}

template<class T>
void DataModelInterface<T>::bubbleSort(bool withUpdatePerSort, bool withSwapUpdates, compareFunction cf)
{
    if (!withUpdatePerSort && !withSwapUpdates)
    {
        DataModelInterface<T>::beginResetModel();
    }
    bool done(false);
    while (!done)
    {
        if (withUpdatePerSort)
        {
            DataModelInterface<T>::beginResetModel();
        }
        done = oneBubbleSort(withSwapUpdates, cf) == 0;
        if (withUpdatePerSort)
        {
            DataModelInterface<T>::endResetModel();
        }
    }
    if (!withUpdatePerSort && !withSwapUpdates)
    {
        DataModelInterface<T>::endResetModel();
    }
}
