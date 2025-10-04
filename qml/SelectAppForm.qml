import QtQuick 2.15
import QtQuick.Shapes 1.5
import "items"

Rectangle
{
    anchors.fill: parent
    id: theSelectAppForm

    function installApp(app_id, installationCode, installText)
    {
        installText.text = qsTr("Please wait")
        CPPQMLAppAndConfigurator.waitMessage(installText.text)
        yacApp.yacappServerGetAPP(app_id,
                                  installationCode,
                                  0,
                                  function(message)
                                  {
                                      installText.text = qsTr("Install")
                                      CPPQMLAppAndConfigurator.hideWaitMessage()
                                      CPPQMLAppAndConfigurator.goodMessage(qsTr("App installed, have fun."), null, null)
                                  },
                                  function(message)
                                  {
                                      installText.text = qsTr("Install")
                                      CPPQMLAppAndConfigurator.hideWaitMessage()
                                      CPPQMLAppAndConfigurator.badMessage(message, null, null)
                                  }
                                  )
    }

    Shape {
        anchors.fill: parent
        ShapePath
        {
            fillGradient: RadialGradient {
                centerX: theSelectAppForm.width / 2
                centerY: theSelectAppForm.height / 2
                centerRadius: theSelectAppForm.height / 2
                focalX: centerX
                focalY: centerY
                GradientStop { position: 0; color: "#F7DDF8" }
                GradientStop { position: 1; color: "#F8FFFD" }
            }
            startX: 0; startY: 0
            PathLine { x: theSelectAppForm.width - 1; y: 0 }
            PathLine { x: theSelectAppForm.width - 1; y: theSelectAppForm.height - 1 }
            PathLine { x: 0; y: theSelectAppForm.height - 1 }
        }
    }

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
        Helper.jsLog("fetched")
        if (jsontext.length < 3)
        {
            return;
        }
        let obj = JSON.parse(jsontext)
        let allApps = obj["allApps"]
        appModel.clear()
        appModel.theAppImages.clear()
        for (let i = 0; i < allApps.length; ++i)
        {
            let app = allApps[i]
            for (let c = 0; c < app.app_images.length; ++c)
            {
                var hash = i + "#" + c
                appModel.theAppImages.set(hash, app.app_images[c])
            }
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

    Item
    {
        id: topItem
        width: parent.width
        height: Constants.designHeightFactor * 371
        z: 1
        YACImage
        {
            id: topImage
            width: parent.width
            height: Constants.designWidthFactor * 250
            source: "https://picsum.photos/" + Math.floor(width) + "/" + Math.floor(height)
            Rectangle
            {
                anchors.fill: parent
                color: "white"
                opacity: 0.7
                visible: installationCodeRectangle.visible
            }
        }
        Rectangle
        {
            id: searchRectangle
            anchors.verticalCenter: topImage.bottom
            x: Constants.designWidthFactor * 36
            width: parent.width - 2 * x
            height: Constants.designHeightFactor * 62
            YACTextInput
            {
                id: searchProjectNeedle
                emptyText.text: qsTr("CODE or Search")
                emptyText.color: "#EEEEEE"
                anchors.fill: parent
                font.pixelSize: Constants.designHeightFactor * 30
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            visible: !installationCodeRectangle.visible
        }
        YACText
        {
            visible: !installationCodeRectangle.visible
            text: "Projekte"
            font.pixelSize: Constants.designHeightFactor * 22
            font.bold: true
            x: Constants.designWidthFactor * 36
            y: Constants.designHeightFactor * 319
        }
    }

    ListModel
    {
        id: appModel
        property var theAppImages: new Map()
    }

    ListView
    {
        id: appView
        clip: true
        visible: appModel.count > 0
        width: parent.width
        property double elemHeight: Constants.designHeightFactor * 100
        anchors.top: topItem.bottom
        anchors.bottom: parent.bottom
        model: appModel
        spacing: radiusSpacing / 2
        property double radiusSpacing: width / 12
        add: Transition {
            ParallelAnimation
            {
                NumberAnimation
                {
                    properties: "x"
                    from: appView.width
                    duration: Constants.slowAnimationDuration
                }
                NumberAnimation
                {
                    properties: "opacity"
                    from: 0
                    to: 1
                    duration: Constants.slowAnimationDuration
                }
                onFinished: Helper.jsLog(appView.ViewTransition.index)
            }
        }
        delegate: Flickable
        {
            id: theFlickable
            property int position: 0
            property string needle: searchProjectNeedle.displayText.toLowerCase()
            property bool matched: needle == "" || app_name.toLowerCase().search(needle) > -1 || search_code.toLowerCase() == needle
            visible: opacity > 0
            opacity: 1
            Behavior on opacity {
                NumberAnimation {
                    duration: Constants.fastAnimationDuration
                }
            }
            height: appView.elemHeight
            Behavior on height {
                NumberAnimation {
                    duration: Constants.fastAnimationDuration
                }
            }
            states: [
                State {
                    when: !matched
                    PropertyChanges
                    {
                        target: theFlickable
                        opacity: 0
                        height: 0
                    }
                }
            ]
            width: appView.width
            contentWidth: theRow.width
            interactive: theRow.width > width

            Row
            {
                id: theRow
                height: appView.elemHeight
                Item
                {
                    height: parent.height
                    width: Constants.designWidthFactor * 36
                }


                Rectangle
                {
                    height: appView.elemHeight
                    width: Constants.designWidthFactor * 266
                    YACText
                    {
                        id: nameText
                        text: app_name
                        anchors.top: parent.top
                        anchors.left: appLogo.left
                        horizontalAlignment: Text.AlignHCenter
                    }
                    YACImage
                    {
                        id: appLogo
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.horizontalCenterOffset: -parent.width / 4
                        anchors.top: nameText.bottom
                        anchors.bottom: parent.bottom
                        width: Math.min(parent.width, height)
                        source: app_logo_url
                    }
                    Rectangle
                    {
                        id: moreRectangle
                        anchors.top: parent.top
                        anchors.left: parent.horizontalCenter
                        anchors.right: parent.right
                        anchors.bottom: parent.verticalCenter
                        color: "silver"
                        visible: app_info_url != ""
                        YACText
                        {
                            anchors.centerIn: parent
                            text: qsTr("more")
                        }
                        MouseArea
                        {
                            anchors.fill: parent
                            onClicked: Qt.openUrlExternally(app_info_url);
                        }
                    }
                    Rectangle
                    {
                        id: installRectangle
                        anchors.top: parent.verticalCenter
                        anchors.left: parent.horizontalCenter
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        color: "green"
                        YACText
                        {
                            id: installText
                            anchors.centerIn: parent
                            text: qsTr("Install")
                        }
                        MouseArea
                        {
                            anchors.fill: parent
                            onClicked: {
                                Helper.jsLog("App selected")
                                if (installation_code_needed)
                                {
                                    installationCodeRectangle.app_id = app_id
                                    installationCodeRectangle.visible = true;
                                    installationCodeRectangle.installText = installText
                                    return
                                }
                                installApp(app_id, "", installText)
                                return
                            }
                        }
                    }

                }
                Repeater
                {
                    property int appIndex: index
                    model: app_images
                    id: theRepeater
                    Row
                    {
                        Item
                        {
                            width: appView.spacing
                            height: appView.elemHeight
                        }
                        YACImage
                        {
                            height: appView.elemHeight
                            source: "image://async/appImage/" + appModel.theAppImages.get(theRepeater.appIndex + "#" + index)
                            MouseArea
                            {
                                anchors.fill: parent
                                onClicked:
                                {
                                    largeImageView.appIndex = theRepeater.appIndex
                                    largeImageView.model = app_images
                                    largeImageItem.opacity = 1
                                    largeImageView.positionViewAtIndex(index, ListView.SnapPosition)
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Item
    {
        id: largeImageItem
        visible: opacity > 0
        opacity: 0
        Behavior on opacity {
            NumberAnimation {
                duration: Constants.slowAnimationDuration
            }
        }
        anchors.fill: parent
        Rectangle
        {
            anchors.fill: parent
            opacity: 0.85
        }
        ListView
        {
            orientation: ListView.Horizontal
            snapMode: ListView.SnapOneItem
            property int appIndex: 0
            anchors.fill: parent
            id: largeImageView
            spacing: width / 20

            delegate: Item
            {
                width: largeImageView.width
                height: largeImageView.height

                YACImage
                {
                    anchors.fill: parent
                    source: "image://async/appImage/" + appModel.theAppImages.get(largeImageView.appIndex + "#" + index)
                }
            }
        }
        YACButton
        {
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            text: qsTr("Close")
            onClicked: largeImageItem.opacity = 0
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
    YACRectangle
    {
        property string app_id: ""
        property var installText: null
        id: installationCodeRectangle
        visible: false
        anchors.fill: parent
        Shape {
            anchors.fill: parent
            ShapePath
            {
                fillGradient: RadialGradient {
                    centerX: theSelectAppForm.width / 2
                    centerY: theSelectAppForm.height / 2
                    centerRadius: theSelectAppForm.height / 2
                    focalX: centerX
                    focalY: centerY
                    GradientStop { position: 0; color: "#F7DDF8" }
                    GradientStop { position: 1; color: "#F8FFFD" }
                }
                startX: 0; startY: 0
                PathLine { x: theSelectAppForm.width - 1; y: 0 }
                PathLine { x: theSelectAppForm.width - 1; y: theSelectAppForm.height - 1 }
                PathLine { x: 0; y: theSelectAppForm.height - 1 }
            }
        }
        Column
        {
            x: Constants.designWidthFactor * 36
            width: parent.width - 2 * x
            anchors.verticalCenter: parent.verticalCenter
            spacing: 1
            Rectangle
            {
                width: parent.width
                height: Constants.designHeightFactor * 62
                YACText
                {
                    anchors.centerIn: parent
                    width: parent.width * 3 / 4
                    text: qsTr("Please enter the installation Code to proceed.")
                    font.pixelSize: Constants.designHeightFactor * 20
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            Rectangle
            {
                width: parent.width
                height: Constants.designHeightFactor * 62
                YACTextInput
                {
                    id: installationCodeInput
                    emptyText.text: qsTr("Installation Code")
                    emptyText.color: "#EEEEEE"
                    anchors.fill: parent
                    font.pixelSize: Constants.designHeightFactor * 30
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
            Rectangle
            {
                width: parent.width
                height: Constants.designHeightFactor * 62
                YACText
                {
                    anchors.centerIn: parent
                    text: qsTr("Verify and Install")
                    font.pixelSize: Constants.designHeightFactor * 30
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: installApp(installationCodeRectangle.app_id,
                                          installationCodeInput.displayText,
                                          installationCodeRectangle.installText)
                }
            }
            Rectangle
            {
                width: parent.width
                height: Constants.designHeightFactor * 62
                YACText
                {
                    anchors.centerIn: parent
                    text: qsTr("Back")
                    font.pixelSize: Constants.designHeightFactor * 30
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        installationCodeInput.text = ""
                        installationCodeRectangle.visible = false
                    }
                }
            }
        }
    }
}
