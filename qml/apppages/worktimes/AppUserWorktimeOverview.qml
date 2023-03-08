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
        YACButtonWithHeader
        {
            id: sinceButton
            width: theOverviewPage.width / 2
            headerText: qsTr("Since")
            text: Helper.formatDate(theOverviewPage.since)
            onClicked: yacApp.pickDate(theOverviewPage.since,
                                       function(newDate){theOverviewPage.since = newDate})
        }
        YACButtonWithHeader
        {
            id: untilButton
            width: theOverviewPage.width / 2
            headerText: qsTr("Until")
            text: Helper.formatDate(theOverviewPage.until)
            onClicked: yacApp.pickDate(theOverviewPage.until,
                                       function(newDate){theOverviewPage.until = newDate})
        }
    }
}
