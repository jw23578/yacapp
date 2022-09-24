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
        onConfigChanged:
        {
            menueEditor.config = yacApp.getMenueConfig(config.menueFilename)
        }
        onMenueFilenameChanged:
        {
            menueEditor.config = yacApp.getMenueConfig(config.menueFilename)
        }

        Component.onCompleted:
        {
            menueEditor.config = yacApp.getMenueConfig(config.menueFilename)
        }
    }
    MenueEditor
    {
        id: menueEditor
        anchors.left: mainForm.left
        anchors.top: mainForm.bottom
        anchors.topMargin: 10
        anchors.right: mainForm.right
        anchors.bottom: parent.bottom
    }


    Component.onCompleted:
    {
        console.log("hello2")
        console.log(yacApp.mainConfig)
        console.log(yacApp.mainConfig.background)
        console.log(yacApp.mainConfig.background.imageFilename)
        console.log(yacApp.mainConfig.background.color)
        console.log(yacApp.mainConfig.content.type)
        console.log(yacApp.mainConfig.content.items.length)

    }
}
