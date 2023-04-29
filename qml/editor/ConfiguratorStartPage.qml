import QtQuick 2.0
import QtQuick.Dialogs 1.3
import "qrc:/qml/items"
import "qrc:/qml/dialogs"
import com.yacapp.recentproject 1.0

Rectangle
{
    id: startPage
    anchors.fill: parent
    signal newProjectLoaded()
    FileDialog
    {
        id: loadProjectDialog
        selectExisting: true
        selectMultiple: false
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
            text: "Last Project: " + configurator.lastProjectName
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
