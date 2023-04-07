#ifndef WORKTIMEOBJECT_H
#define WORKTIMEOBJECT_H

#include "dataobjectinterface.h"
#include <QDateTime>

class WorktimeObject : public DataObjectInterface
{
    Q_OBJECT

    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QDateTime, ts, Ts, QDateTime());
    YACAPPPROPERTY(int, type, Type, 0);
public:
    explicit WorktimeObject(QObject *parent = nullptr);

    enum WorktimeType
    {
        WorkStartType = 1,
        WorkEndType,
        PauseStartType,
        PauseEndType,
        OffSiteWorkStartType,
        OffSiteWorkEndType,
        WorktimeTypeCount
    };

    void fromJSON(QJsonObject &o);

    void assign(const WorktimeObject &other);

    Q_INVOKABLE QString getTypeString() const;
};

#endif // WORKTIMEOBJECT_H
