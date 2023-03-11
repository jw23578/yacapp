import QtQuick 2.0
import QtQuick.Controls 2.15

import "../items"

Rectangle
{
    id: timePicker
    anchors.fill: parent
    property date theDateTime: new Date()

    property var okCallback: null
    function show(dateTime, okCallback)
    {
        visible = true
        timePicker.okCallback = okCallback
        timePicker.theDateTime = dateTime
        timeWheel.set(dateTime)
        timeClock.set(dateTime)
    }
    Row
    {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: theSwipeView.top
        YACButton
        {
            text: qsTr("Clock")
            onClicked:
            {
                timeClock.hourItemVisible = true
                theSwipeView.setCurrentIndex(0)
            }
        }
        YACButton
        {
            text: qsTr("Wheel")
            onClicked: theSwipeView.setCurrentIndex(1)
        }
    }

    SwipeView
    {
        id: theSwipeView
        width: parent.width * 8 / 10
        height: width
        anchors.centerIn: parent
        clip: true
        interactive: false
        YACTimeClock
        {
            id: timeClock
            onTheTimeChanged:
            {
                timePicker.theDateTime = theTime
                timeWheel.set(theTime)
            }
        }
        YACTimeWheel
        {
            id: timeWheel
            onTheTimeChanged:
            {
                timePicker.theDateTime = theTime
                timeClock.set(theTime)
            }
        }
    }


    YACTwoButtonRow
    {
        leftText: qsTr("Ok")
        onLeftClicked:
        {
            timePicker.visible = false
            okCallback(timePicker.theDateTime)
        }
        rightText: qsTr("Abort")
        onRightClicked: timePicker.visible = false
    }
}
