import QtQuick 2.15
import "../../items"
import ".."

AppUserBasePage2
{
    id: worktimePage
    anchors.fill: parent
    property bool workStarted: !isNaN(yacApp.appUserConfig.workStart)
    property bool pauseStarted: !isNaN(yacApp.appUserConfig.pauseStart)
    property bool offSiteWorkStarted: !isNaN(yacApp.appUserConfig.offSiteWorkStart)

    function insertEvent(worktimeType)
    {
        yacApp.appUserInsertWorktime(worktimeType, 0, 0, function(message){}, function(message){})
    }

    YACPageColumn
    {
        YACButton
        {
            width: parent.width
            id: workStartButton
            text:  workStarted ? qsTr("End work\nSince: ") +  Helper.smartFormatDateTime(yacApp.appUserConfig.workStart): qsTr("Start work")
            onClicked:
            {
                let worktimeType = 1;
                if (workStarted)
                {
                    if (pauseStarted)
                    {
                        CPPQMLAppAndConfigurator.badMessage(qsTr("Please end your Pause first"), null, function() {})
                        return;
                    }
                    if (offSiteWorkStarted)
                    {
                        CPPQMLAppAndConfigurator.badMessage(qsTr("Please end your offSiteWork first"), null, function() {})
                        return;
                    }
                    worktimeType = 2;
                    dayRatingPage.worktimeType = worktimeType
                    dayRatingPage.visible = true
                    return
                }
                userMoodPage.worktimeType = worktimeType
                userMoodPage.visible = true
            }
        }
        YACButton
        {
            width: parent.width
            id: pauseStartButton
            text: pauseStarted ? qsTr("End Pause\nsince ") +  Helper.smartFormatDateTime(yacApp.appUserConfig.pauseStart) : qsTr("Start Pause")
            onClicked: {
                if (!workStarted)
                {
                    CPPQMLAppAndConfigurator.badMessage(qsTr("Please start your Work first"), null, function() {})
                    return;
                }
                insertEvent(pauseStarted ? 4 : 3)

            }
        }
        YACButton
        {
            width: parent.width
            id: offSiteWorkStartButton
            text: offSiteWorkStarted ? qsTr("End OffsiteWork\nsince ") +  Helper.smartFormatDateTime(yacApp.appUserConfig.offSiteWorkStart) : qsTr("Start OffsiteWork")
            onClicked:
            {
                if (!workStarted)
                {
                    CPPQMLAppAndConfigurator.badMessage(qsTr("Please start your Work first"), null, function() {})
                    return;
                }
                if (pauseStarted)
                {
                    CPPQMLAppAndConfigurator.badMessage(qsTr("Please end your Pause first"), null, function() {})
                    return;
                }
                insertEvent(offSiteWorkStarted ? 6 : 5)
            }
        }
    }
    YACPageColumn
    {
        parent: content
        centerVertical: false
        anchors.bottom: parent.bottom
        anchors.bottomMargin: height / 5
        YACButton
        {
            width: parent.width
            text: qsTr("Overview")
            onClicked: showOverview()
        }
    }

    Rectangle
    {
        visible: false
        id: userMoodPage
        anchors.fill: parent
        property int worktimeType: 0
        property int dayRating: 0
        function goMood(userMood)
        {
            userMoodPage.visible = false
            yacApp.appUserInsertWorktime(worktimeType, userMood, dayRating, function(message){}, function(message){})
            worktimeType = 0
            dayRating = 0
        }

        YACPageColumn
        {
            YACText
            {
                text: userMoodPage.worktimeType == 1 ? qsTr("How is your Mood today?") : qsTr("How is your Mood now?")
            }
            Repeater
            {
                model: yacApp.moodModel
                YACButton
                {
                    text: modelData
                    width: parent.width
                    onClicked: userMoodPage.goMood(index + 1)
                }
            }
            YACButton
            {
                text: qsTr("Abort")
                width: parent.width
                onClicked: userMoodPage.visible = false
            }
        }
    }

    Rectangle
    {
        visible: false
        id: dayRatingPage
        anchors.fill: parent
        property int worktimeType: 0
        function goDayRating(dayRating)
        {
            userMoodPage.dayRating = dayRating
            userMoodPage.worktimeType = worktimeType
            userMoodPage.visible = true
            dayRatingPage.visible = false;
        }

        YACPageColumn
        {
            YACText
            {
                text: qsTr("How was your Day?")
            }
            Repeater
            {
                model: yacApp.moodModel
                YACButton
                {
                    text: modelData
                    width: parent.width
                    onClicked: dayRatingPage.goDayRating(index + 1)
                }
            }
            YACButton
            {
                text: qsTr("Abort")
                width: parent.width
                onClicked: dayRatingPage.visible = false
            }
        }
    }

    function showOverview()
    {
        overviewLoader.since = Helper.firstInMonth(Helper.currentDateTime())
        overviewLoader.until = Helper.lastInMonth(Helper.currentDateTime())
        yacApp.appUserFetchWorktimes(overviewLoader.since,
                                     overviewLoader.until,
                                     function(message)
                                     {
                                         overviewLoader.sourceComponent = theAppUserWorktimeOverviewComponent
                                     },
                                     function(message){})
    }
    Component
    {
        id: theAppUserWorktimeOverviewComponent
        AppUserWorktimeOverview
        {
            id: theAppUserWorktimeOverview
            onCloseClicked: overviewLoader.sourceComponent = null
        }
    }

    Loader
    {
        id: overviewLoader
        anchors.fill: parent
        property date since: new Date()
        property date until: new Date()
        onLoaded:
        {
            item.since = since
            item.until = until
        }
    }
    AppUserInsertWorktimeBeginEnd
    {
        id: beginEnd
        visible: false
        onCloseClicked: visible = false
    }


    Component.onCompleted:
    {
        Helper.jsLog("worktimestate")
        yacApp.appUserGetWorktimeState(function(message) {},
        function(message) {});
    }
}
