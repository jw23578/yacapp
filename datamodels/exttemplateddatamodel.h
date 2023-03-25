#ifndef EXTTEMPLATEDDATAMODEL_H
#define EXTTEMPLATEDDATAMODEL_H

#include "templateddatamodel.h"

template <class T>
class ExtTemplatedDataModel : public TemplatedDataModel<T>
{
    std::map<QString, T*> id2object;
protected:
    void internalAppend(T *object);

public:
    ExtTemplatedDataModel(QQmlApplicationEngine &engine,
                          const QString &modelName,
                          const QString &objectName,
                          typename TemplatedDataModelInterface<T>::DirectionType const direction);

    T *findById(const QString &id);
};

#include "exttemplateddatamodel.cpp"

#endif // EXTTEMPLATEDDATAMODEL_H
