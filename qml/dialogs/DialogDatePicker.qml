import QtQuick 2.0
import "../items"

Rectangle
{
    id: theDatePicker
    anchors.fill: parent
    MouseArea
    {
        anchors.fill: parent
    }
    property var okCallback: null
    property int hour: 0
    property int minute: 0
    function show(date, okCallback)
    {
        visible = true
        theDatePicker.okCallback = okCallback
        theDatePicker.theDate = date
        theDatePicker.hour = Helper.getHour(date)
        theDatePicker.minute = Helper.getMinute(date)
    }

    property date theDate: Helper.currentDateTime()
    property date firstInMonth: Helper.firstInMonth(theDate)
    property int weekDayOffset: Helper.getDayOfWeek(firstInMonth) - 1
    property int spacing: height / 30
    Column
    {
        anchors.centerIn: parent

        YACText
        {
            width: theDatePicker.width
            text: Helper.nameOfWeekDay(theDatePicker.theDate)
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: Constants.defaultFontPixelSize * Constants.x4largerTextFactor
        }
        YACText
        {
            width: theDatePicker.width
            text: Helper.formatDateShort(theDatePicker.theDate)
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: Constants.defaultFontPixelSize * Constants.x4largerTextFactor
        }
        Item
        {
            height: theDatePicker.spacing
            width: 1
        }
        Row
        {
            YACTextClickable
            {
                width: theDatePicker.width / 3
                text: qsTr("Yesterday")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Constants.defaultFontPixelSize * Constants.x4largerTextFactor
                onClicked: theDatePicker.theDate = Helper.incDays(Helper.currentDateTime(), -1)
            }
            YACTextClickable
            {
                width: theDatePicker.width / 3
                text: qsTr("Today")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Constants.defaultFontPixelSize * Constants.x4largerTextFactor
                onClicked: theDatePicker.theDate = Helper.currentDateTime()
            }
            YACTextClickable
            {
                width: theDatePicker.width / 3
                text: qsTr("Tomorrow")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Constants.defaultFontPixelSize * Constants.x4largerTextFactor
                onClicked: theDatePicker.theDate = Helper.incDays(Helper.currentDateTime(), 1)
            }
        }
        Item
        {
            height: theDatePicker.spacing
            width: 1
        }
        Row
        {
            YACArrowButton
            {
                id: arrowLeftYear
                rotation: -90
                onClicked: theDatePicker.theDate = Helper.incYears(theDatePicker.theDate, -1)
            }

            YACText
            {
                width: theDatePicker.width - 2 * arrowLeftYear.width
                text: Helper.getYear(theDatePicker.theDate)
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Constants.defaultFontPixelSize * Constants.x4largerTextFactor
            }
            YACArrowButton
            {
                id: arrowRightYear
                rotation: 90
                onClicked: theDatePicker.theDate = Helper.incYears(theDatePicker.theDate, +1)
            }

        }
        Item
        {
            height: theDatePicker.spacing
            width: 1
        }
        Row
        {
            YACArrowButton
            {
                id: arrowLeftMonth
                rotation: -90
                onClicked: theDatePicker.theDate = Helper.incMonths(theDatePicker.theDate, -1)
            }
            YACText
            {
                width: theDatePicker.width - 2 * arrowLeftMonth.width
                text: Helper.getMonthNameLongFromDate(theDatePicker.theDate)
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Constants.defaultFontPixelSize * Constants.x4largerTextFactor
            }
            YACArrowButton
            {
                id: arrowRightMonth
                rotation: 90
                onClicked: theDatePicker.theDate = Helper.incMonths(theDatePicker.theDate, +1)
            }
        }
        Item
        {
            height: theDatePicker.spacing
            width: 1
        }
        Row
        {
            Repeater
            {
                model: 7
                YACText
                {
                    width: theDatePicker.width / 7
                    text: Helper.getShortDayName(index + 1)
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
        Repeater
        {
            model: (Helper.getDaysInMonthFromDate(theDatePicker.theDate) + 6) / 7
            Row
            {
                property int rowIndex: index
                Repeater
                {
                    model: 7
                    Item
                    {
                        property date myDate: Helper.incDays(theDatePicker.firstInMonth, rowIndex * 7 + index - theDatePicker.weekDayOffset)
                        width: theDatePicker.width / 7
                        height: theDay.contentHeight * 2
                        Rectangle
                        {
                            anchors.centerIn: parent
                            radius: width / 11
                            width: parent.width * 9 / 10
                            height: parent.height * 9 / 10
                            color: "lightgrey"
                            visible: Helper.getDay(myDate) == Helper.getDay(theDatePicker.theDate) && Helper.getMonth(myDate) == Helper.getMonth(theDatePicker.theDate)
                        }
                        YACText
                        {
                            id: theDay
                            anchors.centerIn: parent
                            text: Helper.getDay(myDate)
                            horizontalAlignment: Text.AlignHCenter
                            color: Helper.getMonth(myDate) == Helper.getMonth(theDatePicker.theDate) ? "black" : "grey"
                        }
                        MouseArea
                        {
                            anchors.fill: parent
                            onClicked: theDatePicker.theDate = myDate
                        }
                    }
                }
            }
        }
    }
    YACTwoButtonRow
    {
        rightText: qsTr("Abort")
        onRightClicked: theDatePicker.visible = false
        leftText: qsTr("Ok")
        onLeftClicked:
        {
            theDatePicker.visible = false
            theDatePicker.okCallback(Helper.createDateTime(Helper.getYear(datePicker.theDate),
                                                           Helper.getMonth(datePicker.theDate),
                                                           Helper.getDay(theDatePicker.theDate),
                                                           datePicker.hour, datePicker.minute))
        }
    }
}
