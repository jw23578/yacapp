import QtQuick 2.15
import ".."
import "../../items"

AppUserBasePage
{
    id: thePage
    property date begin: new Date()
    property date end: new Date()
    YACPageColumn
    {
        id: theColum
        YACComboBoxWithHeader
        {
            id: workTimeType
            width: parent.width
            model: [qsTr("Work"), qsTr("Pause"), qsTr("OffsiteWork")]
        }
        YACDateTimeButton
        {
            datetime: begin
            buttonWidth: parent.width
            headerText: qsTr("Begin")
            onDatetimeChanged: begin = datetime
        }
        YACDateTimeButton
        {
            datetime: end
            buttonWidth: parent.width
            headerText: qsTr("End")
            onDatetimeChanged: end = datetime
        }

    }
    leftText: qsTr("Save")
    onLeftClicked:
    {
        if (begin >= end)
        {
            yacApp.badMessage(qsTr("End needs to be after Begin"), null, null)
            return
        }
        yacApp.appUserInsertWorktimeBeginEnd(workTimeType.currentIndex * 2 + 1,
                                             begin,
                                             end,
                                             function(message) {thePage.visible = false},
                                             function(message) {yacApp.badMessage(message, null, null)})
    }
}
