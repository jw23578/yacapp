import QtQuick 2.0
import QtQuick.Controls 2.15

Item
{
    id: timeWheel
    property date theTime: new Date()
    property alias hour: theHourTumbler.currentIndex
    property int minute: theMinuteTumbler.currentIndex * 5
    onHourChanged: theTime = Helper.setHour(theTime, hour)
    onMinuteChanged: theTime = Helper.setMinute(theTime, minute)
    function set(datetime)
    {
        timeWheel.theTime = datetime
        theHourTumbler.currentIndex = Helper.getHour(datetime)
        theMinuteTumbler.currentIndex = Helper.getMinute(datetime) / 5
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
        YACTumbler
        {
            visibleItemCount: 7
            width: parent.width
            id: theHourTumbler
            height: timeWheel.height - hourText.height
            model: 24
            delegate: Item
            {
                opacity: 1.0 - Math.abs(Tumbler.displacement) / (theHourTumbler.visibleItemCount / 2)
                YACText
                {
                    anchors.centerIn: parent
                    text: index
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
        YACTumbler
        {
            id: theMinuteTumbler
            width: parent.width
            height: timeWheel.height - minuteText.height
            model: 12
            delegate: Item
            {
                opacity: 1.0 - Math.abs(Tumbler.displacement) / (theMinuteTumbler.visibleItemCount / 2)
                YACText
                {
                    anchors.centerIn: parent
                    text: index * 5
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
