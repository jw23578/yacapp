import QtQuick 2.0
import QtQuick.Controls 2.15

import "../items"

Rectangle
{
    id: timePicker
    anchors.fill: parent

    property var okCallback: null
    property int year: 0
    property int month: 0
    property int day: 0
    function show(dateTime, okCallback)
    {
        visible = true
        timePicker.okCallback = okCallback
        timePicker.year = Helper.getYear(dateTime)
        timePicker.month = Helper.getMonth(dateTime)
        timePicker.day = Helper.getDay(dateTime)
        timeWheel.set(Helper.getHour(dateTime), Helper.getMinute(dateTime))
    }

    YACTimeWheel
    {
        id: timeWheel
        width: Math.min(parent.width * 8 / 10, parent.height / 2 * 8 / 10)
        height: width
        anchors.centerIn: parent
    }

    YACTwoButtonRow
    {
        leftText: qsTr("Ok")
        onLeftClicked:
        {
            timePicker.visible = false
            okCallback(Helper.createDateTime(timePicker.year, timePicker.month, timePicker.day, timeWheel.hour, timeWheel.minute))
        }
        rightText: qsTr("Abort")
        onRightClicked: timePicker.visible = false
    }
}
