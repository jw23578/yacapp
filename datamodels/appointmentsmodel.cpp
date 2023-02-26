#include "appointmentsmodel.h"

void AppointmentsModel::internalAppend(AppointmentObject *object)
{
    for (size_t i(0); i < size(); ++i)
    {
        AppointmentObject &o(*getObject(i));
        if (o.appointment_group_id() == object->appointment_group_id())
        {
            o.assign(*object);
            delete object;
            return;
        }
    }
    TemplatedDataModel<AppointmentObject>::internalAppend(object);
}

AppointmentsModel::AppointmentsModel(QQmlApplicationEngine &engine):
    TemplatedDataModel<AppointmentObject>(engine, "AppointmentsModel", "appointment", TemplatedDataModel<AppointmentObject>::forward)
{

}
