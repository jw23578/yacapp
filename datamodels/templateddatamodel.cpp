#include "templateddatamodel.h"

template<class T>
size_t TemplatedDataModel<T>::size() const
{
    return objects.size();
}

template<class T>
T *TemplatedDataModel<T>::getObject(size_t index) const
{
    return objects[index];
}

template<class T>
bool TemplatedDataModel<T>::canAppend(T *object) const
{
    return true;
}

template<class T>
void TemplatedDataModel<T>::internalAppend(T *object)
{
    objects.push_back(object);
}

template<class T>
T *TemplatedDataModel<T>::previousObject(int index) const
{
    if (index == 0)
    {
        return 0;
    }
    return objects[index - 1];
}

template <class T>
TemplatedDataModel<T>::TemplatedDataModel(QQmlApplicationEngine &engine,
                                          const QString &modelName,
                                          const QString &objectName,
                                          typename DataModelInterface<T>::DirectionType const direction):
    DataModelInterface<T>(engine,
                          modelName,
                          objectName,
                          direction)
{

}

template<class T>
void TemplatedDataModel<T>::internalClear()
{
    for (auto o : objects)
    {
        delete o;
    }
    objects.clear();
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
    T *a(objects[i1]);
    objects[i1] = objects[i2];
    objects[i2] = a;
    if (update)
    {
        DataModelInterface<T>::endMoveRows();
    }
}
