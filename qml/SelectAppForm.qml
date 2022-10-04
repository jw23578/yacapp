import QtQuick 2.15
import "items"
import "../HttpFunctions.js" as Http

YACRectangle
{
    color: "orange"
    visible: yacApp.globalConfig.projectID == ""
    anchors.fill: parent

    function allAppsLoaded(yacApp, data)
    {
        var jsontext = data.responseText
        if (jsontext.length < 3)
        {
            return;
        }
        let obj = JSON.parse(jsontext)
        let allApps = obj["allApps"]
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
        Http.request(yacApp, yacApp.allAppsUrl, allAppsLoaded, errorCallback, abortCallback)
    }

    ListModel
    {
        id: appModel
    }

    ListView
    {
        id: appView
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
                onClicked: console.log("App selected")
            }
        }
    }
    onVisibleChanged:
    {
        if (visible)
        {
            console.log("Fetching on Visible Changed")
            fetchKnownApps()
        }
    }

    Component.onCompleted:
    {
        if (visible)
        {
            console.log("Fetching on Component completed")
            fetchKnownApps()
        }
    }
}
