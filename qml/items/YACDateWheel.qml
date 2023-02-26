import QtQuick 2.0
import QtQuick.Controls 2.15

Item
{
    id: dateWheel
    property int yearStart: 1950
    property int year: theYearTumbler.currentIndex + yearStart
    property int month: theMonthTumbler.currentIndex + 1
    property int day: theDayTumbler.currentIndex + 1
    function set(year, month, day)
    {
        theYearTumbler.currentIndex = year - yearStart
        theMonthTumbler.currentIndex = month - 1
        theDayTumbler.currentIndex = day - 1
    }

    Column
    {
        id: yearColumn
        width: parent.width / 3
        YACText
        {
            id: yearText
            text: qsTr("Year")
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Tumbler
        {
            width: parent.width
            id: theYearTumbler
            height: dateWheel.height - yearText.height
            model: 100
            delegate: Item
            {
                Rectangle
                {
                    anchors.fill: parent
                    anchors.margins: 1
                    border.width: 1
                    border.color: "black"
                    color: theYearTumbler.currentIndex == index ? Constants.goodColor : "white"
                    YACText
                    {
                        anchors.centerIn: parent
                        text: dateWheel.yearStart + index
                    }
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: theYearTumbler.currentIndex = index
                }
            }
        }
    }
    Column
    {
        id: monthColumn
        width: parent.width / 3
        x: width
        YACText
        {
            id: monthText
            text: qsTr("Month")
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Tumbler
        {
            id: theMonthTumbler
            width: parent.width
            height: dateWheel.height - monthText.height
            model: 12
            delegate: Item
            {
                Rectangle
                {
                    anchors.fill: parent
                    anchors.margins: 1
                    border.width: 1
                    border.color: "black"
                    color: theMonthTumbler.currentIndex == index ? Constants.goodColor : "white"
                    YACText
                    {
                        anchors.centerIn: parent
                        text: Helper.getMonthNameLong(index + 1)
                    }
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: theMonthTumbler.currentIndex = index
                }
            }
        }
    }
    Column
    {
        id: dayColumn
        width: parent.width / 3
        x: width * 2
        YACText
        {
            id: dayText
            text: qsTr("Day")
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Tumbler
        {
            id: theDayTumbler
            width: parent.width
            height: dateWheel.height - dayText.height
            model: Helper.getDaysInMonth(dateWheel.year, dateWheel.month)
            delegate: Item
            {
                Rectangle
                {
                    anchors.fill: parent
                    anchors.margins: 1
                    border.width: 1
                    border.color: "black"
                    color: theDayTumbler.currentIndex == index ? Constants.goodColor : "white"
                    YACText
                    {
                        anchors.centerIn: parent
                        text: index + 1
                    }
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: theDayTumbler.currentIndex = index
                }
            }
        }
    }
}
