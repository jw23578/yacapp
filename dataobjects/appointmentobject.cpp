#include "appointmentobject.h"

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
