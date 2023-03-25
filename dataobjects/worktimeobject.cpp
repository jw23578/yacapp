#include "worktimeobject.h"
#include <QJsonObject>

WorktimeObject::WorktimeObject(QObject *parent)
    : DataObjectInterface{parent}
{

}

void WorktimeObject::fromJSON(QJsonObject &o)
{
    setId(o[tableFields.id].toString());
    QString tsString(o[tableFields.ts].toString());
    setTs(QDateTime::fromString(tsString, Qt::DateFormat::ISODateWithMs));
    setType(o[tableFields.type].toString().toInt());
}

void WorktimeObject::assign(const WorktimeObject &other)
{
    setId(other.id());
    setTs(other.ts());
    setType(other.type());
}

QString WorktimeObject::getTypeString() const
{
    switch(type())
    {
    case WorkStartType: return tr("Workstart");
    case WorkEndType: return tr("Workend");
    case PauseStartType: return tr("Pausestart");
    case PauseEndType: return tr("Pauseend");
    case OffSiteWorkStartType: return tr("Offsiteworkstart");
    case OffSiteWorkEndType: return tr("Offsiteworkend");
    }
    return "nix";
}
