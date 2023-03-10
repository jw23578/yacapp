#ifndef WORKTIMEMAINOBJECT_H
#define WORKTIMEMAINOBJECT_H

#include "dataobjectinterface.h"
#include <QDateTime>
#include "datamodels/templateddatamodel.h"
#include "dataobjects/worktimeobject.h"

class WorktimeMainObject : public DataObjectInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QDateTime, begin_ts, begin_ts, QDateTime());
    YACAPPPROPERTY(QDateTime, end_ts, end_ts, QDateTime());

    YACAPPPROPERTY(int, brutto_work_minutes, brutto_work_minutes, 0);
    YACAPPPROPERTY(int, brutto_pause_minutes, brutto_pause_minutes, 0);

    YACAPPPROPERTY(int, netto_work_minutes, netto_work_minutes, 0);
    YACAPPPROPERTY(int, netto_pause_minutes, netto_pause_minutes, 0);
public:
    explicit WorktimeMainObject(QObject *parent = nullptr);
    TemplatedDataModel<WorktimeObject> subentries;

    Q_INVOKABLE QAbstractItemModel *getSubentries();
};

#endif // WORKTIMEMAINOBJECT_H
