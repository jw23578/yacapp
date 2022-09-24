import QtQuick 2.0
import QtQuick.Dialogs 1.3
import "../items"

Rectangle
{
    id: startPage
    anchors.fill: parent
    FileDialog
    {
        id: fileDialog
        selectExisting: true
        selectMultiple: false
        nameFilters: [ "yacApp-Project-Files (*.yacapp)" ]
        onFileUrlChanged:
        {
            yacApp.loadNewProject(fileUrl)
            configurator.lastProjectFilename = fileUrl
            configurator.lastProjectName = yacApp.globalConfig.projectName
            configurator.save()
            startPage.visible = false
        }
    }
    Row
    {
        spacing: 1
        YACButton
        {
            text: qsTr("New Project")
        }
        YACButton
        {
            text: qsTr("Load Project")
            onClicked: fileDialog.open()
        }
        YACButton
        {
            visible: configurator.lastProjectName != ""
            text: "Last Project: " + configurator.lastProjectName
            onClicked:
            {
                yacApp.loadNewProject(configurator.lastProjectFilename)
                startPage.visible = false
            }
        }
    }
}
