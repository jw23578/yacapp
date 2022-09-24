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

    Loader
    {
        id: mainFormLoader
        width: 640
        height: 480
    }

    Component
    {
        id: mainFormComponent
        MainForm
        {
            id: mainForm
            width: 640
            height: 480
            config: yacApp.mainConfig
            onCurrentItemChanged: editorFormLoader.item.config = config
            clip: true
        }
    }
    Loader
    {
        id: editorFormLoader
        anchors.top: parent.top
        anchors.left: mainFormLoader.right
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom

    }

    Component
    {
        id: editorFormComponent
        EditorForm
        {
            id: editorForm
            config: yacApp.mainConfig
            global: yacApp.globalConfig
            anchors.fill: parent
            onOpenOtherProject: startPage.visible = true
            onLoadConfig: mainFormLoader.item.openFilename(filename)
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
    }
    MenueEditor
    {
        id: menueEditor
        anchors.left: mainFormLoader.left
        anchors.top: mainFormLoader.bottom
        anchors.topMargin: 10
        anchors.right: mainFormLoader.right
        anchors.bottom: parent.bottom
    }

    ConfiguratorStartPage
    {
        id: startPage
        onNewProjectLoaded:
        {
            menueEditor.config = yacApp.getMenueConfig(yacApp.mainConfig.menueFilename)
            mainFormLoader.source = ""
            editorFormLoader.source = ""
            mainFormLoader.sourceComponent = mainFormComponent
            editorFormLoader.sourceComponent = editorFormComponent
        }
    }
}
