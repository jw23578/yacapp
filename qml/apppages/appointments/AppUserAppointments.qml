import QtQuick 2.15
import "../../items"
import ".."

AppUserBasePage
{
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
        anchors.bottom: closeButton.top
        anchors.margins: Constants.defaultMargin
        reuseItems: true
        model: AppointmentsModel
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
        }

    }

    YACButton
    {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        text: qsTr("Add Appointment")
        onClicked: insertAppointmentsLoader.sourceComponent = insertAppointmentsComponent
    }

    Loader
    {
        anchors.fill: parent
        id: insertAppointmentsLoader
    }
}
