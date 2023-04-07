import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.0
import "qml"
import "qrc:/qml/items"
import "qml/editor"
import "qml/dialogs"

Window {
    width: 1280
    height: 1000
    visible: true
    title: qsTr("YAC-App Designer")

    DefaultsSetter {}

    Loader
    {
        id: mainFormLoader
        width: 430
        height: 932
    }

    Component
    {
        id: mainFormComponent
        MainForm
        {
            id: mainForm
            width: 430
            height: 932
            config: yacApp.mainConfig
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
                        width: parent.width / 3 - 1
                        text: theSwipeView.currentIndex == 0 ? qsTr("Page Editor visible") : qsTr("Show Page Editor")
                        onClicked: theSwipeView.currentIndex = 0
                    }
                    YACButton
                    {
                        width: parent.width / 3 - 1
                        text: theSwipeView.currentIndex == 1 ? qsTr("Menue Editor visible") : qsTr("Show Page Editor")
                        onClicked: theSwipeView.currentIndex = 1
                    }
                    YACButton
                    {
                        width: parent.width / 3 - 1
                        text: theSwipeView.currentIndex == 2 ? qsTr("Design Editor visible") : qsTr("Show Design Editor")
                        onClicked: theSwipeView.currentIndex = 2
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
                ConfiguratorDesignEditor
                {
                    id: designEditor
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
    FileDialog
    {
        id: theImageFileDialog
        selectFolder: false
        selectMultiple: true
        nameFilters: [ "Image files (*.jpg *.png)", "All files (*)" ]
        property var okCallback: null
        onAccepted: okCallback(fileUrls)
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
        function onAddImageSignal(okCallback)
        {
            theImageFileDialog.okCallback = okCallback
            theImageFileDialog.open()
        }
    }
    Component.onCompleted:
    {
        Constants.appTotalWidth = mainFormLoader.width
        Constants.appTotalHeight = mainFormLoader.height
    }

    ConfiguratorAppImagesPage
    {
        id: appImages
        visible: true
    }
}
