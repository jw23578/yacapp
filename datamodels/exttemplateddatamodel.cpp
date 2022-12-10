#include "exttemplateddatamodel.h"

template <class T>
ExtTemplatedDataModel<T>::ExtTemplatedDataModel(QQmlApplicationEngine &engine,
                                                const QString &modelName,
                                                const QString &objectName,
                                                typename DataModelInterface<T>::DirectionType const direction):
    TemplatedDataModel<T>(engine,
                          modelName,
                          objectName,
                          direction)
{

}

template<class T>
void ExtTemplatedDataModel<T>::internalAppend(T *object)
{
    TemplatedDataModel<T>::internalAppend(object);
    id2object[object->id()] = object;
}


template<class T>
T *ExtTemplatedDataModel<T>::findById(const QString &id)
{
    auto it(id2object.find(id));
    if (it == id2object.end())
    {
        return 0;
    }
    return it->second;
}
