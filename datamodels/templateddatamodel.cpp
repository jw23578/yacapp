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
