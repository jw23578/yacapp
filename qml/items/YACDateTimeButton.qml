import QtQuick 2.15

Row
{
    property double buttonWidth: 100
    property alias headerText: datePart.headerText
    property date datetime: new Date()
    YACDateButton
    {
        id: datePart
        width: buttonWidth / 2
        theDate: datetime
        onTheDateChanged: datetime = theDate
    }
    YACTimeButton
    {
        theTime: datetime
        width: buttonWidth / 2
        onTheTimeChanged: datetime = theTime
    }

}
