import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import "qml"
import "qml/items"

Window {
    width: 480
    height: 640
    visible: true
    title: qsTr("Hello World")

    MainForm
    {
        anchors.fill: parent
        config: yacApp.mainConfig
    }

    SelectAppForm
    {
        id: selectApp
    }

    SequentialAnimation
    {
        id: startUpAni
        PauseAnimation
        {
            duration: 3000
        }
        NumberAnimation
        {
            target: splashscreen
            property: "opacity"
            to: 0
            duration: 500
        }
    }
    YACRectangle
    {
        visible: opacity > 0
        id: splashscreen
        anchors.fill: parent
        color: "silver"
        YACText
        {
            anchors.centerIn: parent
            width: parent.width * 3 / 4
            text: "This is the yacApp"
            font.bold: true
            font.pixelSize: width / 10
            horizontalAlignment: Text.AlignHCenter
        }
    }

    YACBadMessageForm
    {
        id: badMessage
    }

    Connections
    {
        target: yacApp
        function onBadMessage(message)
        {
            badMessage.show(message)
        }
    }

    Component.onCompleted:
    {
        startUpAni.start()
        console.log("hello")
        console.log("ProjectID: " + yacApp.globalConfig.projectID)
        console.log(yacApp.mainConfig)
        console.log(yacApp.mainConfig.background)
        console.log(yacApp.mainConfig.background.imageFilename)
        console.log(yacApp.mainConfig.background.color)
        console.log(yacApp.mainConfig.content.type)
        console.log(yacApp.mainConfig.content.items.length)
        console.log(yacApp.mainConfig.content.items[1].height)
    }
}
