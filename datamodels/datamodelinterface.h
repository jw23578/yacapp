#ifndef DATAMODELINTERFACE_H
#define DATAMODELINTERFACE_H

#include <QAbstractItemModel>
#include <QQmlApplicationEngine>
#include "dataobjects/dataobjectinterface.h"

template <class T>
class DataModelInterface : public QAbstractItemModel
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
    virtual size_t size() const = 0;
    virtual T *getObject(size_t index) const = 0;
    virtual void internalAppend(T *object) = 0;
    virtual T *previousObject(int index) const = 0;
public:
    explicit DataModelInterface(QQmlApplicationEngine &engine,
                                const QString &modelName,
                                const QString &objectName,
                                DirectionType const direction);

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void append(T *object);
};

#include "datamodelinterface.cpp"

#endif // DATAMODELINTERFACE_H
