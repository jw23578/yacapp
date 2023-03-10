#ifndef WORKTIMEOBJECT_H
#define WORKTIMEOBJECT_H

#include "dataobjectinterface.h"
#include "yacAppAndServer/tablefields.h"
#include <QDateTime>

class WorktimeObject : public DataObjectInterface
{
    Q_OBJECT
    TableFields tableFields;

    YACAPPPROPERTY(QString, id, id, "");
    YACAPPPROPERTY(QDateTime, ts, ts, QDateTime());
    YACAPPPROPERTY(int, type, type, 0);
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

    Q_INVOKABLE QString getTypeString() const;
};

#endif // WORKTIMEOBJECT_H
