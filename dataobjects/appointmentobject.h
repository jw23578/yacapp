#ifndef APPOINTMENTOBJECT_H
#define APPOINTMENTOBJECT_H

#include "dataobjectinterface.h"
#include <QDateTime>
#include "yacAppAndServer/tablefields.h"

class AppointmentObject : public DataObjectInterface
{    
    Q_OBJECT
    TableFields tableFields;

    YACAPPPROPERTY(QString, id, id, "");
    YACAPPPROPERTY(QString, appointment_group_id, appointment_group_id, "");
    YACAPPPROPERTY(QString, appointment_template_id, appointment_template_id, "");
    YACAPPPROPERTY(QString, caption, caption, "");
    YACAPPPROPERTY(QString, description, description, "");
    YACAPPPROPERTY(QDateTime, start_datetime, start_datetime, QDateTime());
    YACAPPPROPERTY(QDateTime, end_datetime, end_datetime, QDateTime());
    YACAPPPROPERTY(QString, creater_id, creater_id, "");
    YACAPPPROPERTY(int, max_bookable_count, max_bookable_count, 0);
    YACAPPPROPERTY(QDateTime, bookable_since_datetime, bookable_since_datetime, QDateTime());
    YACAPPPROPERTY(QDateTime, bookable_until_datetime, bookable_until_datetime, QDateTime());
    YACAPPPROPERTY(int, booking_credits, booking_credits, 0);
    YACAPPPROPERTY(bool, visible_for_everybody, visible_for_everybody, false);

public:
    AppointmentObject();

    void assign(const AppointmentObject &other);

    void fromJSON(QJsonObject &o);
};

#endif // APPOINTMENTOBJECT_H
