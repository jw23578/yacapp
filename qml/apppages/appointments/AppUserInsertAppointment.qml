import QtQuick 2.15
import "../../items"
import ".."

AppUserBasePage
{
    signal appointmentSaved();
    property date theStartDateTime: Helper.createDateTime2(Helper.currentDateTime(), 8, 0)
    property date theEndDateTime: Helper.createDateTime2(Helper.currentDateTime(), 8, 0)
    Flickable
    {
        anchors.bottom: parent.closeButton.top
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        YACPageColumn
        {
            YACLineEditWithHeader
            {
                id: theCaption
                headerText: qsTr("Caption")
            }
            YACButtonWithHeader
            {
                id: startDateButton
                headerText: qsTr("Beginn")
                text: Helper.formatDate(theStartDateTime) + "   " + Helper.formatTime(theStartDateTime)
                onClicked: yacApp.pickDateTime(theStartDateTime,
                                               function(selectedDate)
                {
                    theStartDateTime = selectedDate
                })
            }
            YACButtonWithHeader
            {
                id: endDateButton
                headerText: qsTr("Ende")
                text: Helper.formatDate(theEndDateTime) + "   " + Helper.formatTime(theEndDateTime)
                onClicked: yacApp.pickDateTime(theEndDateTime,
                                               function(selectedDate)
                {
                    theEndDateTime = selectedDate
                })
            }
        }
    }
    YACButton
    {
        text: qsTr("Save")
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        onClicked:
        {
            var appointment_group_id = ""
            var appointment_template_id = ""
            yacApp.appUserInsertAppointment(appointment_group_id,
                                            appointment_template_id,
                                            theCaption.displayText,
                                            theStartDateTime,
                                            theEndDateTime,
                                            function(message){appointmentSaved()},
                                            function(message){console.log(message)})
        }
    }
}
