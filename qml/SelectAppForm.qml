import QtQuick 2.15
import "items"
import "../HttpFunctions.js" as Http

YACRectangle
{
    color: "orange"
    visible: yacApp.globalConfig.projectID == ""
    anchors.fill: parent


    PauseAnimation
    {
        id: reloadAni
        duration: 60 * 1000
        onStopped:
        {
            fetchKnownApps()
            if (!visible)
            {
                return
            }
            reloadAni.start()
        }
    }

    function allAppsLoaded(yacApp, data)
    {
        console.log("fetched")
        var jsontext = data.responseText
        if (jsontext.length < 3)
        {
            return;
        }
        let obj = JSON.parse(jsontext)
        let allApps = obj["allApps"]
        appModel.clear()
        for (let i = 0; i < allApps.length; ++i)
        {
           let app = allApps[i]
           appModel.append(app)
        }
    }

    function errorCallback(yacApp, data)
    {

    }

    function abortCallback(yacApp, data)
    {

    }

    function fetchKnownApps()
    {
        Http.request(yacApp, yacApp.allAppsBaseUrl + "allApps.json", allAppsLoaded, errorCallback, abortCallback)
    }

    ListModel
    {
        id: appModel
    }

    ListView
    {
        id: appView
        visible: appModel.count > 0
        clip: true
        width: parent.width * 2 / 4
        height: parent.height * 3 / 4
        anchors.centerIn: parent
        model: appModel
        spacing: radiusSpacing / 2
        property double radiusSpacing: width / 12
        delegate: YACRectangle
        {
            color: projectColor
            border.width: 1
            border.color: "black"
            width: appView.width
            height: width
            radius: appView.radiusSpacing
            radiusTopLeft: index == 0
            radiusTopRight: radiusTopLeft
            radiusBottomLeft: index == appModel.count - 1
            radiusBottomRight: radiusBottomLeft
            YACText
            {
                id: nameText
                text: projectName
                anchors.top: parent.top
                anchors.topMargin: appView.spacing
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
            }
            YACImage
            {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: nameText.bottom
                anchors.bottom: parent.bottom
                width: height
                source: projectLogo
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    console.log("App selected")
                    yacApp.downloadApp(yacApp.allAppsBaseUrl, projectID)
                }
            }
        }
    }
    onVisibleChanged:
    {
        if (visible)
        {
            fetchKnownApps()
            reloadAni.start()
        }
        else
        {
            reloadAni.stop()
        }
    }

    Component.onCompleted:
    {
        if (visible)
        {
            fetchKnownApps()
            reloadAni.start()
        }
    }
    YACRectangle
    {
        anchors.centerIn: parent
        width: parent.width * 3 / 4
        height: width
        visible: appModel.count == 0
        YACText
        {
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("No known Apps yet")
        }
    }
    Connections
    {
        target: yacApp
        function onAppDownloadError(errorMessage)
        {
            console.log("Console: " + errorMessage)
            yacApp.badMessage(errorMessage)
        }
        function onAppDownloadSuccess()
        {
            console.log("App Downloaded")
            visible = false
        }
    }
}
