import QtQuick 2.15
import "items"

YACRectangle
{
    color: "orange"
    anchors.fill: parent
    id: theSelectAppForm


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

    function allAppsLoaded(jsontext)
    {
        console.log("fetched")
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

    function errorCallback(data)
    {

    }


    function fetchKnownApps()
    {
        yacApp.yacappServerGetAllAPPs(allAppsLoaded, errorCallback)
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
            color: app_color_name
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
                text: app_name
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
                source: app_logo_url
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    console.log("App selected")
                    yacApp.yacappServerGetAPP(app_id,
                                              0,
                                              function(message)
                                              {
                                                  yacApp.goodMessage(qsTr("App installed, have fun."), null, null)
                                              },
                                              function(message)
                                              {
                                                  yacApp.badMessage(message, null, null)
                                              }
                                              )
                    return
                    yacApp.downloadApp(Constants.allAppsBaseUrl, app_id)
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
            yacApp.badMessage(errorMessage, null, null)
        }
        function onAppDownloadSuccess()
        {
            console.log("App Downloaded")
//            visible = false
        }
    }
}
