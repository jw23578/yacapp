import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import "qml"
import "qrc:/qml/items"
import "qml/editor"
import "qml/dialogs"

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
//            onCurrentItemChanged:
//            {
//                if (config != null && editorFormLoader.item != null)
//                {
//                    editorFormLoader.item.config = config
//                }
//            }
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
        Item
        {
            anchors.fill: parent
            Column
            {
                id: topColumn
                width: parent.width
                spacing: projectName.height
                Row
                {
                    spacing: 1
                    width: parent.width
                    YACButton
                    {
                        width: parent.width / 3 - 2
                        text: qsTr("Other Project")
                        onClicked: startPage.visible = true
                    }
                    YACButton
                    {
                        width: parent.width / 3 - 2
                        text: qsTr("Save Project")
                        onClicked: yacApp.saveCurrentProject()
                    }
                    YACButton
                    {
                        width: parent.width / 3 - 2
                        text: qsTr("Deploy Project")
                        onClicked: deployPage.visible = true
                    }
                }
                YACProjectText
                {
                    id: projectName
                    width: parent.width
                    text: qsTr("Project: ") + yacApp.globalConfig.projectName
                }
                Row
                {
                    spacing: 1
                    width: parent.width
                    YACButton
                    {
                        width: parent.width / 2 - 1
                        text: theSwipeView.currentIndex == 0 ? qsTr("Page Editor visible") : qsTr("Show Page Editor")
                        onClicked: theSwipeView.currentIndex = 0
                    }
                    YACButton
                    {
                        width: parent.width / 2 - 1
                        text: theSwipeView.currentIndex == 1 ? qsTr("Menue Editor visible") : qsTr("Menue Page Editor")
                        onClicked: theSwipeView.currentIndex = 1
                    }
                }
                Item
                {
                    width: 1
                    height: 1
                }
            }
            SwipeView
            {
                id: theSwipeView
                anchors.top: topColumn.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                clip: true
                ConfiguratorPageEditor
                {
                    id: editorForm
                    config: yacApp.mainConfig
                    global: yacApp.globalConfig
                    onLoadConfig:
                    {
                        config = yacApp.getConfig(filename)
                        mainFormLoader.item.openFilename(filename)
                    }
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
                ConfiguratorMenueEditor
                {
                    id: menueEditor
                    config: yacApp.getMenueConfig(yacApp.mainConfig.menueFilename)
                }
            }

        }
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
            //            menueEditor.config = yacApp.getMenueConfig(yacApp.mainConfig.menueFilename)
            mainFormLoader.source = ""
            editorFormLoader.source = ""
            mainFormLoader.sourceComponent = mainFormComponent
            editorFormLoader.sourceComponent = editorFormComponent
        }
    }
    DialogAddFile
    {
        id: dialogAddFile
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
    Connections
    {
        target: configurator
        function onAddFileSignal(okCallback)
        {
            dialogAddFile.okCallback = okCallback
            dialogAddFile.open()
        }
    }
}
