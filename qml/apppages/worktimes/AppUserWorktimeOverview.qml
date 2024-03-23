import QtQuick 2.15
import ".."
import "../../items"

AppUserBasePage
{
    showCloseButton: true
    property date since: new Date()
    property date until: new Date()
    id: theOverviewPage
    Row
    {
        id: sinceUntilButtons
        YACDateButton
        {
            id: sinceButton
            width: theOverviewPage.width / 2
            headerText: qsTr("Since")
            theDate: theOverviewPage.since
        }
        YACDateButton
        {
            id: untilButton
            width: theOverviewPage.width / 2
            headerText: qsTr("Until")
            theDate: until
        }
    }
    ListView
    {
        id: theListview
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: sinceUntilButtons.bottom
        anchors.bottom: buttonRow.top
        width: parent.width * Constants.defaultWidthFactor
        model: WorktimesModel
        clip: true
        spacing: 1
        delegate: Rectangle
        {
            id: theRectangle
            width: theListview.width
            height: theColumn.height
            Behavior on height
            {
                NumberAnimation {
                    duration: Constants.fastAnimationDuration
                }
            }
            clip: true

            color: "yellow"
            MouseArea
            {
                anchors.fill: parent
                onClicked: theRepeater.model = theRepeater.model == 0 ? worktime.getSubentries() : 0
            }
            Column
            {
                id: theColumn
                width: parent.width
                Row
                {
                    id: theRow
                    YACText
                    {
                        id: dateText
                        text: Helper.formatDateShort(worktime.begin_ts)
                    }
                    YACText
                    {
                        text: qsTr("Begin: ") + Helper.formatTime(worktime.begin_ts)
                    }
                    YACText
                    {
                        text: qsTr("End: ") + Helper.formatTime(worktime.end_ts)
                    }
                }
                Row
                {
                    YACText
                    {
                        text: qsTr("Brutto D: ") + worktime.brutto_work_minutes + qsTr(" P: ") + worktime.brutto_pause_minutes
                    }
                }
                Row
                {
                    YACText
                    {
                        text: qsTr("Netto D: ") + worktime.netto_work_minutes + qsTr(" P: ") + worktime.netto_pause_minutes + qsTr(" AP: ") + worktime.autopause_minutes
                    }
                }

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
                                text: entry.getTypeString() + " " + Helper.formatDateTime(entry.ts) + " " + entry.type
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
