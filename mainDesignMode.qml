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
        width: 480
        height: 640
    }

    Component
    {
        id: mainFormComponent
        MainForm
        {
            id: mainForm
            width: 480
            height: 640
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
            onDeployProjectClicked: deployPage.visible = true
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
    ConfiguratorDeployPage
    {
        id: deployPage
        visible: false
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

    YACBadMessageForm
    {
        id: badMessage
    }
    YACGoodMessageForm
    {
        id: goodMessage
    }
    YACYesNoQuestionForm
    {
        id: yesNoQuestion
    }

    Connections
    {
        target: yacApp
        function onBadMessage(message, itemToFocus, okCallback)
        {
            badMessage.show(message, itemToFocus, okCallback)
        }
        function onGoodMessage(message, itemToFocus, okCallback)
        {
            goodMessage.show(message, itemToFocus, okCallback)
        }
        function onYesNoQuestion(question, itemToFocus, yesCallback, noCallback)
        {
            yesNoQuestion.show(question, itemToFocus, yesCallback, noCallback)
        }
    }


}
