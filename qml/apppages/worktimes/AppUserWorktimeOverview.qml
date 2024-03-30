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
                                         function(message)
                                         {
                                         },
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
                YACText
                {
                    id: dateText
                    text: Helper.formatDateLong(worktime.begin_ts)
                }
                Row
                {
                    id: theRow
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
