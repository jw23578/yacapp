#include "appointmentobject.h"
#include <QJsonObject>

AppointmentObject::AppointmentObject()
    : DataObjectInterface{0}
{

}

void AppointmentObject::assign(const AppointmentObject &other)
{
    setid(other.id());
    setappointment_group_id(other.appointment_group_id());
    setappointment_template_id(other.appointment_template_id());
    setcaption(other.caption());
    setdescription(other.description());
    setstart_datetime(other.start_datetime());
    setend_datetime(other.end_datetime());
    setcreater_id(other.creater_id());
    setmax_bookable_count(other.max_bookable_count());
    setbookable_since_datetime(other.bookable_since_datetime());
    setbookable_until_datetime(other.bookable_until_datetime());
    setbooking_credits(other.booking_credits());
    setvisible_for_everybody(other.visible_for_everybody());
}

void AppointmentObject::fromJSON(QJsonObject &o)
{
    setid(o[tableFields.id].toString());
    setappointment_group_id(o[tableFields.appointment_group_id].toString());
    setcaption(o[tableFields.caption].toString());
    setstart_datetime(QDateTime::fromString(o[tableFields.start_datetime].toString(), Qt::DateFormat::ISODateWithMs));
    setend_datetime(QDateTime::fromString(o[tableFields.end_datetime].toString(), Qt::DateFormat::ISODateWithMs));
    setvisible_for_everybody(o[tableFields.visible_for_everybody].toString() == "t");
    setcreater_id(o[tableFields.creater_id].toString());
}
