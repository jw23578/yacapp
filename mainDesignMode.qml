import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs
import "qml"
import "qrc:/qml/items"
import "qml/editor"
import "qml/dialogs"
import "qml/apppages"

Window {
    width: 1280
    height: 1000
    visible: true
    title: yacApp.applicationTitle

    DefaultsSetter {}

    Rectangle
    {
        id: theAppRectangle
        x: 0
        y: 0
        width: mainFormLoader.x * 2  + mainFormLoader.width
        height: mainFormLoader.y * 2 + mainFormLoader.height
        color: "gray"
        Loader
        {
            id: mainFormLoader
            x:10
            y: 10
            width: 430
            height: 932
        }
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
        anchors.left: theAppRectangle.right
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
                        useInConfigurator: true
                        width: parent.width / 3 - 2
                        text: qsTr("Other Project")
                        onClicked: startPage.visible = true
                    }
                    YACButton
                    {
                        useInConfigurator: true
                        width: parent.width / 3 - 2
                        text: qsTr("Save Project")
                        onClicked: configurator.saveCurrentProject()
                    }
                    YACButton
                    {
                        useInConfigurator: true
                        width: parent.width / 3 - 2
                        text: qsTr("Deploy Project")
                        onClicked: deployPage.visible = true
                    }
                }
                YACProjectText
                {
                    useInConfigurator: true
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
                        useInConfigurator: true
                        width: parent.width / 3 - 1
                        text: theSwipeView.currentIndex == 0 ? qsTr("Page Editor visible") : qsTr("Show Page Editor")
                        onClicked: theSwipeView.currentIndex = 0
                    }
                    YACButton
                    {
                        useInConfigurator: true
                        width: parent.width / 3 - 1
                        text: theSwipeView.currentIndex == 1 ? qsTr("Menue Editor visible") : qsTr("Show Menue Editor")
                        onClicked: theSwipeView.currentIndex = 1
                    }
                    YACButton
                    {
                        useInConfigurator: true
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
                }
                ConfiguratorMenueEditor
                {
                    id: menueEditor
                    config: yacApp.getMenueConfig(yacApp.mainConfig.menueFilename)
                }
                ConfiguratorDesignEditor
                {
                    id: designEditor
                    global: yacApp.globalConfig
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
        fileMode: FileDialog.OpenFiles
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
        target: CPPQMLAppAndConfigurator
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
        function onTakePhoto(squared, circled, target)
        {
            photoLoader.circled = circled
            photoLoader.squared = squared
            photoLoader.target = target
            photoLoader.sourceComponent = photoComponent
        }
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

    Component
    {
        id: photoComponent
        YACPhoto
        {
            onAbortClicked:
            {
                photoLoader.sourceComponent = null
            }
            onOkClicked: function(image)
            {
                target.source = ""
                console.log("image.source: " + image.source)
                target.source = image.source
                photoLoader.sourceComponent = null
            }
        }
    }

    Loader
    {
        id: photoLoader
        property bool squared: false
        property bool circled: false
        property var target: null
        anchors.fill: parent
        onLoaded:
        {
            item.circled = circled
            item.squared = squared
        }
    }

    Component.onCompleted:
    {
        Constants.appTotalWidth = mainFormLoader.width
        Constants.appTotalHeight = mainFormLoader.height
    }

//    ConfiguratorAppImagesPage
//    {
//        id: appImages
//        visible: true
//    }
}
