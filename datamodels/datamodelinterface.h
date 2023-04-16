#ifndef DATAMODELINTERFACE_H
#define DATAMODELINTERFACE_H

#include <QAbstractItemModel>
#include <QQmlApplicationEngine>
#include "datamodelqobjectinterface.h"

template <class T>
class DataModelInterface : public DataModelQObjectInterface
{
public:
    enum DirectionType
    {
        forward,
        reverse
    };
private:
    static bool first;
    const QString modelName;
    const QString objectName;
    const DirectionType direction;

protected:
    enum OLMRoles {
        ObjectRole = Qt::UserRole + 1,
        DefaultObjectRole
    };
    virtual QHash<int, QByteArray> customRoleNames() const;
    virtual QVariant customData(int row, int role) const;
    virtual T *internalGetObject(size_t index) const = 0;
    virtual void internalRemoveByIndex(const size_t index) = 0;
    virtual void internalAppend(T *object) = 0;
    virtual bool canAppend(T *object) const = 0;
    virtual T *previousObject(int index) const = 0;
    virtual void internalClear() = 0;
    virtual void swap(size_t i1, size_t i2, bool update) = 0;
public:
    explicit DataModelInterface(QQmlApplicationEngine &engine,
                                const QString &modelName,
                                const QString &objectName,
                                DirectionType const direction);
    explicit DataModelInterface(const QString &objectName,
                                DirectionType const direction);

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool append(T *object);
    void removeByIndex(const size_t index);
    void clear();
    const T& get(const size_t index) const;
    const T& operator[](const size_t index) const;

    typedef std::function<bool(const T &,const T &)> compareFunction;
    size_t oneBubbleSort(bool withSwapUpdates, compareFunction cf);
    void bubbleSort(bool withUpdatePerSort, bool withSwapUpdates, compareFunction cf);
};

#include "datamodelinterface.cpp"

#endif // DATAMODELINTERFACE_H
