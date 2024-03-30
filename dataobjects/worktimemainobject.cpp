#include "worktimemainobject.h"

WorktimeMainObject::WorktimeMainObject(QObject *parent)
    : DataObjectInterface(parent),
      subentries("entry")
{
}

QAbstractItemModel *WorktimeMainObject::getSubentries()
{
    return &subentries;
}

void WorktimeMainObject::assign(const WorktimeMainObject &other)
{
    setId(other.id());
    setbegin_ts(other.begin_ts());
    setend_ts(other.end_ts());
    setbrutto_pause_minutes(other.brutto_pause_minutes());
    setbrutto_work_minutes(other.brutto_work_minutes());
    setnetto_pause_minutes(other.netto_pause_minutes());
    setnetto_work_minutes(other.netto_work_minutes());
    setautopause_minutes(other.autopause_minutes());
}
