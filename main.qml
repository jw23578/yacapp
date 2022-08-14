import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import "qml"

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


    Component.onCompleted:
    {
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
