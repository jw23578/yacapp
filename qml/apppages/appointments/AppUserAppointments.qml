import QtQuick 2.15
import "../../items"
import ".."

AppUserBasePage
{
    multiMenueButton.visible: true
    multiMenueButton.model: [{caption: qsTr("Add Appointment")}]
    multiMenueButton.onClicked:
    {
        console.log("caption: " + caption)
        if (caption == qsTr("Add Appointment"))
        {
            insertAppointmentsLoader.sourceComponent = insertAppointmentsComponent
        }
    }
    multiMenueButton.hide: insertAppointmentsLoader.sourceComponent != null

    Component
    {
        id: insertAppointmentsComponent
        AppUserInsertAppointment
        {
            onCloseClicked: insertAppointmentsLoader.sourceComponent = null
            onAppointmentSaved: insertAppointmentsLoader.sourceComponent = null
        }
    }
    ListView
    {
        id: theListview
        clip: true
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: buttonRow.top
        anchors.margins: Constants.defaultMargin
        reuseItems: true
        model: AppointmentsModel
        displaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 1000 }
        }
        remove: Transition {
            NumberAnimation { property: "opacity"; to: 0; duration: 1000 }
        }
        spacing: 1
        delegate: Rectangle
        {
            color: index % 2 ? "silver" : "lightgrey"
            width: parent.width
            height: 100
            Column
            {
                width: parent.width
                YACText
                {
                    text: appointment.caption
                }
                YACText
                {
                    property date start: appointment.start_datetime
                    property date end: appointment.end_datetime
                    property bool sameDay: Helper.sameDay(start, end)
                    text: sameDay ? Helper.formatDate(start) + " " + Helper.formatTime(start) + " - " + Helper.formatTime(end) :
                                    Helper.formatDateTime(start) + " - " + Helper.formatDateTime(end)
                }
                YACText
                {
                    text: appointment.visible_for_everybody ? qsTr("Visible for everybody") : qsTr("This appointment is only for me")
                }
            }
            YACButton
            {
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                text: qsTr("Delete")
                onClicked:
                {
                    if (appointment.creater_id != yacApp.appUserConfig.id)
                    {
                        yacApp.badMessage(qsTr("You only can delete appointments that you created."), null, null);
                        return
                    }

                    yacApp.yesNoQuestion(qsTr("Delete Appointment?"),
                                         null,
                                         function()
                                         {


                                             yacApp.appUserDeleteAppointment(appointment.id,
                                                                             function(message)
                                                                             {
                                                                                 console.log(message)
                                                                             },
                                                                             function(message)
                                                                             {
                                                                                 console.log(message)
                                                                             }
                                                                             )
                                         },
                                         function() {})
                }
            }
        }

    }

    Loader
    {
        anchors.fill: parent
        id: insertAppointmentsLoader
    }
}
