#include "templateddatamodel.h"

template<class T>
size_t TemplatedDataModel<T>::size() const
{
    return objects.size();
}

template<class T>
T *TemplatedDataModel<T>::internalGetObject(size_t index) const
{
    return objects.getByIndex(index);
}

template<class T>
bool TemplatedDataModel<T>::canAppend(T *object) const
{
    T *alreadyAdded(objects.getById(object->id()));
    if (alreadyAdded)
    {
        alreadyAdded->assign(*object);
        return false;
    }
    return true;
}

template<class T>
void TemplatedDataModel<T>::internalRemoveByIndex(const size_t index)
{
    if (index >= objects.size())
    {
        return;
    }
    size_t visibleIndex(index);
    if (DataModelInterface<T>::getDirection() == DataModelInterface<T>::reverse)
    {
        visibleIndex = size() - 1 - index;
    }
    DataModelInterface<T>::beginRemoveRows(QModelIndex(), visibleIndex, visibleIndex);
    objects.deleteByIndex(index);
    DataModelInterface<T>::endRemoveRows();
}

template<class T>
void TemplatedDataModel<T>::internalAppend(T *object)
{
    objects.add(object->id(), object);
}

template<class T>
T *TemplatedDataModel<T>::previousObject(int index) const
{
    if (index == 0)
    {
        return 0;
    }
    return objects.getByIndex(index - 1);
}

template <class T>
TemplatedDataModel<T>::TemplatedDataModel(QQmlApplicationEngine &engine,
                                          const QString &modelName,
                                          const QString &objectName,
                                          typename DataModelInterface<T>::DirectionType const direction):
    DataModelInterface<T>(engine,
                          modelName,
                          objectName,
                          direction),
    objects(true)
{

}

template<class T>
TemplatedDataModel<T>::TemplatedDataModel(const QString &objectName,
                                          const typename DataModelInterface<T>::DirectionType direction):
    DataModelInterface<T>(objectName,
                          direction)
  ,
    objects(true)
{

}

template<class T>
T *TemplatedDataModel<T>::getById(const QString &id)
{
    return objects.getById(id);
}

template<class T>
void TemplatedDataModel<T>::removeById(const QString &id)
{
    size_t index(objects.indexById(id));
    if (index >= objects.size())
    {
        return;
    }
    this->removeByIndex(index);
}

template<class T>
void TemplatedDataModel<T>::internalClear()
{
    objects.clear();
    moreDataAvailable = true;
}

template<class T>
void TemplatedDataModel<T>::swap(size_t i1,
                                 size_t i2,
                                 bool update)
{
    if (i2 <= i1)
    {
        return;
    }
    if (update)
    {
        DataModelInterface<T>::beginMoveRows(QModelIndex(), i2, i2, QModelIndex(), i1);
    }
    objects.swap(i1, i2);
    if (update)
    {
        DataModelInterface<T>::endMoveRows();
    }
}

template<class T>
void TemplatedDataModel<T>::internalDeleteById(const QString &id)
{
    size_t index(objects.indexById(id));
    if (index == static_cast<size_t>(-1))
    {
        return;
    }
    DataModelInterface<T>::beginRemoveRows(QModelIndex(), index, index);
    objects.deleteByIndex(index);
    DataModelInterface<T>::endRemoveRows();
}

template<class T>
void TemplatedDataModel<T>::fetchMore(const QModelIndex &parent)
{
    if (theFetchMoreFunction)
    {
        theFetchMoreFunction(*this);
    }
}

template<class T>
bool TemplatedDataModel<T>::canFetchMore(const QModelIndex &parent) const
{
    return moreDataAvailable;
}

template<class T>
void TemplatedDataModel<T>::setFetchMoreFuntion(fetchMoreFunctionPointer fmfp)
{
    theFetchMoreFunction = fmfp;
    moreDataAvailable = true;
}

template<class T>
void TemplatedDataModel<T>::lastObjectFetched()
{
    moreDataAvailable = false;
}
