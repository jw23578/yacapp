#include "appointmentsmodel.h"

AppointmentsModel::AppointmentsModel(QQmlApplicationEngine &engine):
    TemplatedDataModel<AppointmentObject>(engine, "AppointmentsModel", "appointment", TemplatedDataModel<AppointmentObject>::forward)
{

}

void AppointmentsModel::removeById(const QString &id)
{
    for (size_t i(0); i < size(); ++i)
    {
        AppointmentObject *o(getObject(i));
        if (o->id() == id)
        {
            internalRemove(o);
        }
    }
}
