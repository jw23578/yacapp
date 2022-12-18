import QtQuick 2.15
import "../items"

Rectangle
{
    color: "green"
    anchors.fill: parent
    property bool workStarted: !isNaN(yacApp.appUserConfig.workStart)
    property bool pauseStarted: !isNaN(yacApp.appUserConfig.pauseStart)
    property bool offSiteWorkStarted: !isNaN(yacApp.appUserConfig.offSiteWorkStart)
    YACPageColumn
    {
        YACButton
        {
            width: parent.width
            id: workStartButton
            text:  workStarted ? qsTr("End work\nsince ") +  Helper.smartFormatDateTime(yacApp.appUserConfig.workStart): qsTr("Start work")
            onClicked:
            {
                let worktimeType = 1;
                if (workStarted)
                {
                    if (pauseStarted)
                    {
                        yacApp.badMessage(qsTr("Please end your Pause first"), null, function() {})
                        return;
                    }
                    if (offSiteWorkStarted)
                    {
                        yacApp.badMessage(qsTr("Please end your offSiteWork first"), null, function() {})
                        return;
                    }

                    worktimeType = 2;
                }
                yacApp.appUserInsertWorktime(worktimeType, function(message){}, function(message){})
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
                    yacApp.badMessage(qsTr("Please start your Work first"), null, function() {})
                    return;
                }

                let worktimeType = 3;
                if (pauseStarted)
                {
                    worktimeType = 4;
                }
                yacApp.appUserInsertWorktime(worktimeType, function(message){}, function(message){})
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
                    yacApp.badMessage(qsTr("Please start your Work first"), null, function() {})
                    return;
                }
                if (pauseStarted)
                {
                    yacApp.badMessage(qsTr("Please end your Pause first"), null, function() {})
                    return;
                }
                let worktimeType = 5;
                if (offSiteWorkStarted)
                {
                    worktimeType = 6;
                }
                yacApp.appUserInsertWorktime(worktimeType, function(message){}, function(message){})
            }
        }
    }

    Component.onCompleted:
    {
        console.log("worktimestate")
        yacApp.appUserGetWorktimeState(function(message) {},
        function(message) {});
    }
}
