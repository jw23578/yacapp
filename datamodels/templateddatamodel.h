#ifndef TEMPLATEDDATAMODEL_H
#define TEMPLATEDDATAMODEL_H

#include "datamodelinterface.h"
#include "container/indexedpointercontainer.h"

template <class T>
class TemplatedDataModel : public DataModelInterface<T>
{
    IndexedPointerContainer<T, QString> objects;
protected:
    T *internalGetObject(size_t index) const override;
    bool canAppend(T *object) const override;
    void internalRemoveByIndex(const size_t index) override;
    void internalAppend(T *object)  override;
    T *previousObject(int index) const override;
    void internalClear() override;
    void swap(size_t i1, size_t i2, bool update) override;
    void internalDeleteById(const QString &id) override;

public:
    TemplatedDataModel(QQmlApplicationEngine &engine,
                       const QString &modelName,
                       const QString &objectName,
                       typename DataModelInterface<T>::DirectionType const direction = DataModelInterface<T>::forward);

    TemplatedDataModel(const QString &objectName,
                       typename DataModelInterface<T>::DirectionType const direction = DataModelInterface<T>::forward);

    virtual ~TemplatedDataModel() {}

    size_t size() const override;
    T *getById(const QString &id);
    void removeById(const QString &id);
};

#include "templateddatamodel.cpp"

#endif // TEMPLATEDDATAMODEL_H
