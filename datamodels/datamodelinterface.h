#ifndef DATAMODELINTERFACE_H
#define DATAMODELINTERFACE_H

#include <QAbstractItemModel>
#include <QQmlApplicationEngine>
#include "dataobjects/dataobjectinterface.h"

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
    std::vector<DataObjectInterface*> objects;

    enum OLMRoles {
        ObjectRole = Qt::UserRole + 1,
        DefaultObjectRole
    };

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

    void append(DataObjectInterface *object);
};

#endif // DATAMODELINTERFACE_H
