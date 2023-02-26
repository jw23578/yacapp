import QtQuick 2.0
import QtQuick.Controls 2.15

Item
{
    id: timeWheel
    property alias hour: theHourTumbler.currentIndex
    property int minute: theMinuteTumbler.currentIndex * 5
    function set(hour, minute)
    {
        theHourTumbler.currentIndex = hour
        theMinuteTumbler.currentIndex = minute / 5
    }

    Column
    {
        id: hourColumn
        width: parent.width / 2
        YACText
        {
            id: hourText
            text: qsTr("Hour")
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Tumbler
        {
            width: parent.width
            id: theHourTumbler
            height: timeWheel.height - hourText.height
            model: 24
            delegate: Item
            {
                Rectangle
                {
                    anchors.fill: parent
                    anchors.margins: 1
                    border.width: 1
                    border.color: "black"
                    color: theHourTumbler.currentIndex == index ? Constants.goodColor : "white"
                    YACText
                    {
                        anchors.centerIn: parent
                        text: index
                    }
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: theHourTumbler.currentIndex = index
                }
            }
        }
    }
    Column
    {
        id: minuteColumn
        width: parent.width / 2
        x: width
        YACText
        {
            id: minuteText
            text: qsTr("Minute")
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Tumbler
        {
            id: theMinuteTumbler
            width: parent.width
            height: timeWheel.height - minuteText.height
            model: 12
            delegate: Item
            {
                Rectangle
                {
                    anchors.fill: parent
                    anchors.margins: 1
                    border.width: 1
                    border.color: "black"
                    color: theMinuteTumbler.currentIndex == index ? Constants.goodColor : "white"
                    YACText
                    {
                        anchors.centerIn: parent
                        text: index * 5
                    }
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: theMinuteTumbler.currentIndex = index
                }
            }
        }
    }
}
