import QtQuick 2.15

Row
{
    id: sinceUntilButtons
    property alias sinceDate: sinceButton.theDate
    property alias untilDate: untilButton.theDate
    property int stepMonths: 1
    property int stepDays: 1
    signal clicked;
    spacing: 1
    function calcDates(day, months)
    {
        if (months)
        {
            sinceButton.theDate = Helper.incMonths(sinceButton.theDate, months)
            untilButton.theDate = Helper.incDays(Helper.incMonths(Helper.incDays(untilButton.theDate, 1), months), -1)
        }
    }

    YACArrowButton
    {
        id: leftButton
        width: sinceButton.buttonHeight
        height: sinceButton.buttonHeight
        imageRotation: -90
        anchors.bottom: parent.bottom
        radius: height / 4
        onClicked:
        {
            calcDates(-stepDays, -stepMonths)
            sinceUntilButtons.clicked()
        }
    }
    YACDateButton
    {
        id: sinceButton
        width: parent.width / 2 - leftButton.width - 1
        headerText: qsTr("Since")
        onDateChanged: sinceUntilButtons.clicked()
    }
    YACDateButton
    {
        id: untilButton
        width: parent.width / 2 - rightButton.width - 1
        headerText: qsTr("Until")
        onDateChanged: sinceUntilButtons.clicked()
    }
    YACArrowButton
    {
        id: rightButton
        height: sinceButton.buttonHeight
        width: sinceButton.buttonHeight
        imageRotation: 90
        anchors.bottom: parent.bottom
        radius: height / 4
        onClicked:
        {
            calcDates(stepDays, stepMonths)
            sinceUntilButtons.clicked()
        }
    }
}
