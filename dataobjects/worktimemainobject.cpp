#include "worktimemainobject.h"

WorktimeMainObject::WorktimeMainObject(QObject *parent)
    : DataObjectInterface{parent},
      subentries("entry")
{

}

QAbstractItemModel *WorktimeMainObject::getSubentries()
{
    return &subentries;
}
