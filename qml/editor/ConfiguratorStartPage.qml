import QtQuick 2.0
import QtQuick.Dialogs
import "qrc:/qml/items"
import "qrc:/qml/dialogs"
import com.yacapp.recentproject 1.0

ConfiguratorPage
{
    id: startPage
    signal newProjectLoaded()

    FileDialog
    {
        id: loadProjectDialog
        fileMode: FileDialog.OpenFile
        nameFilters: [ "yacApp-Project-Files (*.yacapp)" ]
        onAccepted:
        {
            configurator.loadProjectFromFile(fileUrl)
            newProjectLoaded()
            startPage.visible = false
        }
    }
    property int buttonHeight: parent.height / 3.6
    property int buttonWidth: parent.width / 3

    Item
    {
        anchors.fill: contentItem

        Column
        {
            id: leftColumn
            width: startPage.buttonWidth
            spacing: parent.height / 40
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: -parent.width / 4
            anchors.bottom: parent.bottom
            anchors.bottomMargin: spacing
            YACButton
            {
                width: startPage.buttonWidth
                height: startPage.buttonHeight
                text: qsTr("Create New Project")
                onClicked: newProjectPage.open()
            }
            YACButton
            {
                width: startPage.buttonWidth
                height: startPage.buttonHeight
                text: qsTr("Load Project")
                onClicked: loadProjectDialog.open()
            }
            YACButton
            {
                width: startPage.buttonWidth
                height: startPage.buttonHeight
                visible: configurator.lastProjectName != ""
                text: "Last Project:\r\n\r\n" + configurator.lastProjectName
                source: configurator.lastProjectLogoUrl
                onClicked:
                {
                    configurator.loadProjectFromFile(configurator.lastProjectFilename)
                    newProjectLoaded()
                    startPage.visible = false
                }
            }
        }
        ListView
        {
            clip: true
            width: startPage.buttonWidth
            spacing: parent.height / 40
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: parent.width / 4
            anchors.top: leftColumn.top
            anchors.bottom: leftColumn.bottom
            model: configurator.recentItemCount
            delegate: YACButton
            {
                width: startPage.buttonWidth
                height: startPage.buttonHeight
                text: configurator.recentProjects[index].projectName
                source: configurator.recentProjects[index].logoUrl
                showLoadingImage: true
                onClicked:
                {
                    configurator.loadProjectFromFile(configurator.recentProjects[index].projectFilename)
                    newProjectLoaded()
                    startPage.visible = false
                }
            }
        }

        ConfiguratorNewProject
        {
            id: newProjectPage
            onCreated:
            {
                newProjectLoaded()
                startPage.visible = false
            }
        }
    }
}
