#ifndef TEMPLATEDDATAMODEL_H
#define TEMPLATEDDATAMODEL_H

#include "datamodelinterface.h"

template <class T>
class TemplatedDataModel : public DataModelInterface<T>
{
    std::vector<T*> objects;
protected:
    size_t size() const override;
    T *getObject(size_t index) const override;
    void internalAppend(T *object)  override;
    T *previousObject(int index) const override;
    void internalClear();

public:
    TemplatedDataModel(QQmlApplicationEngine &engine,
                       const QString &modelName,
                       const QString &objectName,
                       typename DataModelInterface<T>::DirectionType const direction);
};

#include "templateddatamodel.cpp"

#endif // TEMPLATEDDATAMODEL_H
