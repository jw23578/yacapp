import QtQuick 2.15
import ".."
import "../../items"

AppUserBasePage
{
    showCloseButton: true
    property date since: new Date()
    property date until: new Date()
    id: theOverviewPage
    YACDateSpanButton
    {
        id: spanButton
        width: parent.width
        sinceDate: theOverviewPage.since
        untilDate: theOverviewPage.until
        onClicked:
            yacApp.appUserFetchWorktimes(sinceDate,
                                         untilDate,
                                         function(message){},
                                         function(message){})
    }

    ListView
    {
        id: theListview
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Constants.defaultMargin
        anchors.rightMargin: Constants.defaultMargin
        anchors.top: spanButton.bottom
        anchors.bottom: buttonRow.top
        model: WorktimesModel
        clip: true
        spacing: 1
        delegate: Item
        {
            id: theRectangle
            width: theListview.width
            height: theColumn.height
            clip: true

            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    if (theRepeater.model == 0)
                    {
                        theRepeater.model = worktime.getSubentries();
                        return
                    }
                    subentriesItem.height == 0 ? subentriesItem.height = subentriesColumn.height : subentriesItem.height = 0
                }
            }
            Column
            {
                id: theColumn
                width: parent.width
                YACText
                {
                    anchors.horizontalCenter: parent.horizontalCenter
                    id: dateText
                    text: Helper.formatDateLong(worktime.begin_ts)
                    font.pixelSize: Constants.defaultFontPixelSize * Constants.smallerTextFactor
                }
                Row
                {
                    YACText
                    {
                        text: Helper.formatTime(worktime.begin_ts)
                    }
                    YACText
                    {
                        text:  " - "
                    }
                    YACText
                    {
                        text: Helper.formatTimeWithDateShortIfNeeded(worktime.end_ts, worktime.begin_ts)
                        id: letzterText
                    }
                    Item
                    {
                        height: 1
                        width: theColumn.width / 2 - letzterText.x - letzterText.width
                    }

                    YACText
                    {
                        text: qsTr("Ergebnis: ")
                        id: ergebnisText
                    }

                    YACText
                    {
                        width: ergebnisText.width
                        horizontalAlignment: Text.AlignHCenter
                        text: Helper.formatMinutesToDaysAndHours(worktime.netto_work_minutes)
                    }
                }
                Row
                {
                    anchors.horizontalCenter: parent.horizontalCenter
                    YACText
                    {
                        font.pixelSize: Constants.defaultFontPixelSize * Constants.smallerTextFactor
                        text: qsTr(" Dauer: ") + Helper.formatMinutesToDaysAndHours(worktime.brutto_work_minutes)
                    }
                    YACText
                    {
                        font.pixelSize: Constants.defaultFontPixelSize * Constants.smallerTextFactor
                        text: qsTr(" Pause: ") + Helper.formatMinutesToDaysAndHours(worktime.netto_pause_minutes)
                    }
                }

                Item
                {
                    Behavior on height
                    {
                        NumberAnimation {
                            duration: Constants.fastAnimationDuration
                        }
                    }
                    id: subentriesItem
                    width: parent.width
                    height: subentriesColumn.height
                    clip: true
                    Column
                    {
                        id: subentriesColumn
                        width: parent.width
                        Repeater
                        {
                            id: theRepeater
                            model: 0
                            Item
                            {
                                width: parent.width
                                height: entryRow.height
                                Row
                                {
                                    id: entryRow
                                    Item
                                    {
                                        width: entryTypeText.height * (entry.type < 3 ? 1 : entry.type < 5 ? 3 : 2)
                                        height: entryTypeText.height
                                    }

                                    YACText
                                    {
                                        id: entryTypeText
                                        text: entry.getTypeString() + " " + Helper.formatDateTime(entry.ts)
                                    }
                                    height: 20
                                }
                                YACButton
                                {
                                    anchors.right: parent.right
                                    anchors.top: parent.top
                                    text: qsTr("Delete")
                                    visible: entry.type == 1 || entry.type == 3 || entry.type == 5
                                    onClicked:
                                    {
                                        yacApp.appUserDeleteWorktime(entry.id, function(message){}, function(message){})
                                    }
                                }
                            }
                        }
                    }
                }
                Rectangle
                {
                    height: 1
                    width: parent.width
                    color: Constants.buttonPrimaryColor
                }
            }
        }
    }
    AppUserInsertWorktimeBeginEnd
    {
        id: beginEnd
        visible: false
        onCloseClicked: visible = false
    }
    leftText: qsTr("Add Begin/End")
    onLeftClicked: beginEnd.visible = true
}
