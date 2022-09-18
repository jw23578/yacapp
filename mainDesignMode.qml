import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import "qml"
import "qml/editor"

Window {
    width: 1280
    height: 1000
    visible: true
    title: qsTr("YAC-App Designer")

    MainForm
    {
        id: mainForm
        width: 640
        height: 480
        config: yacApp.mainConfig
        onCurrentItemChanged: editorForm.config = config
        clip: true
    }
    EditorForm
    {
        id: editorForm
        config: yacApp.mainConfig
        global: yacApp.globalConfig
        anchors.top: parent.top
        anchors.left: mainForm.right
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onLoadConfig: mainForm.openFilename(filename)
    }


    Component.onCompleted:
    {
        yacApp.loadNewProject("/home/jw78/wes23/")

        console.log("hello2")
        console.log(yacApp.mainConfig)
        console.log(yacApp.mainConfig.background)
        console.log(yacApp.mainConfig.background.imageFilename)
        console.log(yacApp.mainConfig.background.color)
        console.log(yacApp.mainConfig.content.type)
        console.log(yacApp.mainConfig.content.items.length)

    }
}
