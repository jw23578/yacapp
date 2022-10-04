import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import "qml"
import "qml/items"

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    MainForm
    {
        anchors.fill: parent
        config: yacApp.mainConfig
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

    Component.onCompleted:
    {
        startUpAni.start()
        console.log("hello")
        console.log(yacApp.mainConfig)
        console.log(yacApp.mainConfig.background)
        console.log(yacApp.mainConfig.background.imageFilename)
        console.log(yacApp.mainConfig.background.color)
        console.log(yacApp.mainConfig.content.type)
        console.log(yacApp.mainConfig.content.items.length)
        console.log(yacApp.mainConfig.content.items[1].height)
    }
}
