import QtQuick 2.15
import ".."
import "../../items"

AppUserBasePage
{
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
                        text: Helper.formatDate(worktime.begin_ts)
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
                        text: qsTr("Netto D: ") + worktime.netto_work_minutes + qsTr(" P: ") + worktime.netto_pause_minutes
                    }
                }

                Repeater
                {
                    id: theRepeater
                    model: 0
                    Row
                    {
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
                }
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked: theRepeater.model = theRepeater.model == 0 ? worktime.getSubentries() : 0
            }
        }
    }
}
