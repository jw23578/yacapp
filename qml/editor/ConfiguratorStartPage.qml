import QtQuick 2.0
import QtQuick.Dialogs 1.3
import "qrc:/qml/items"
import "qrc:/qml/dialogs"

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
            yacApp.loadNewProject(fileUrl)
            configurator.lastProjectFilename = fileUrl
            configurator.lastProjectName = yacApp.globalConfig.projectName
            configurator.save()
            newProjectLoaded()
            startPage.visible = false
        }
    }
    Row
    {
        spacing: 1
        YACButton
        {
            text: qsTr("New Project")
            onClicked: newProjectPage.open()
        }
        YACButton
        {
            text: qsTr("Load Project")
            onClicked: loadProjectDialog.open()
        }
        YACButton
        {
            visible: configurator.lastProjectName != ""
            text: "Last Project: " + configurator.lastProjectName
            onClicked:
            {
                yacApp.loadNewProject(configurator.lastProjectFilename)
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
