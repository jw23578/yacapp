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
        anchors.bottom: parent.buttonRow.top
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        YACPageColumn
        {
            id: theColumn
            YACLineEditWithHeader
            {
                id: theCaption
                headerText: qsTr("Caption")
            }
            Row
            {
                YACButtonWithHeader
                {
                    width: theColumn.width / 2
                    id: startDateButton
                    headerText: qsTr("Begin")
                    text: Helper.formatDate(theStartDateTime)
                    onClicked: yacApp.pickDate(theStartDateTime,
                                               function(selectedDate)
                                               {
                                                   theStartDateTime = selectedDate
                                               })
                }
                YACButtonWithHeader
                {
                    width: theColumn.width / 2
                    id: startTimeButton
                    text: Helper.formatTime(theStartDateTime)
                    onClicked: yacApp.pickTime(theStartDateTime,
                                                   function(selectedDate)
                                                   {
                                                       theStartDateTime = selectedDate
                                                   })
                }
            }
            Row
            {
                YACButtonWithHeader
                {
                    width: theColumn.width / 2
                    id: endDateButton
                    headerText: qsTr("End")
                    text: Helper.formatDate(theEndDateTime)
                    onClicked: yacApp.pickDate(theEndDateTime,
                                                   function(selectedDate)
                                                   {
                                                       theEndDateTime = selectedDate
                                                   })
                }
                YACButtonWithHeader
                {
                    width: theColumn.width / 2
                    id: endTimeButton
                    text: Helper.formatTime(theEndDateTime)
                    onClicked: yacApp.pickTime(theEndDateTime,
                                                   function(selectedDate)
                                                   {
                                                       theEndDateTime = selectedDate
                                                   })
                }
            }
            YACComboBoxWithHeader
            {
                id: visible_for_everybody
                headerText: qsTr("Visibility")
                model: [qsTr("Only for me"), qsTr("For everybody")]
            }
        }
    }
    leftText: qsTr("Save")
    onLeftClicked:
    {
        var appointment_group_id = ""
        var appointment_template_id = ""
        yacApp.appUserInsertAppointment(appointment_group_id,
                                        appointment_template_id,
                                        theCaption.displayText,
                                        theStartDateTime,
                                        theEndDateTime,
                                        visible_for_everybody.currentIndex == 1,
                                        function(message){appointmentSaved()},
                                        function(message){console.log(message)})
    }
}
