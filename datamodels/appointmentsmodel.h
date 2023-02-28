#ifndef APPOINTMENTSMODEL_H
#define APPOINTMENTSMODEL_H

#include "templateddatamodel.h"
#include "dataobjects/appointmentobject.h"

class AppointmentsModel : public TemplatedDataModel<AppointmentObject>
{
protected:    
    void internalAppend(AppointmentObject *object)  override;

public:
    AppointmentsModel(QQmlApplicationEngine &engine);

    void removeById(const QString &id);
};

#endif // APPOINTMENTSMODEL_H