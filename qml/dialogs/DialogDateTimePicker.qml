import QtQuick 2.0
import QtQuick.Controls 2.15

import "../items"

Rectangle
{
    id: dateTimePicker
    signal okClicked(date selectedDateTime)
    signal abortClicked()
    anchors.fill: parent

    property var okCallback: null
    function show(dateTime, okCallback)
    {
        visible = true
        dateTimePicker.okCallback = okCallback
        timeWheel.set(Helper.getHour(dateTime), Helper.getMinute(dateTime))
        dateWheel.set(Helper.getYear(dateTime), Helper.getMonth(dateTime), Helper.getDay(dateTime))
    }
    onAbortClicked: visible = false
    onOkClicked:
    {
        visible = false
        okCallback(selectedDateTime)
    }

    YACDateWheel
    {
        id: dateWheel
        width: Math.min(parent.width * 8 / 10, parent.height / 2 * 8 / 10)
        height: width
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -height / 2
    }

    YACTimeWheel
    {
        id: timeWheel
        anchors.left: dateWheel.left
        anchors.right: dateWheel.right
        anchors.top: dateWheel.bottom
        height: dateWheel.height
    }

    YACTwoButtonRow
    {
        leftText: qsTr("Ok")
        onLeftClicked: okClicked(Helper.createDateTime(dateWheel.year, dateWheel.month, dateWheel.day, timeWheel.hour, timeWheel.minute))
        rightText: qsTr("Abort")
        onRightClicked: abortClicked()
    }
}
