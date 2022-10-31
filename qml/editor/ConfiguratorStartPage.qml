import QtQuick 2.0
import QtQuick.Dialogs 1.3
import "../items"

Rectangle
{
    id: startPage
    anchors.fill: parent
    signal newProjectLoaded()
    FileDialog
    {
        id: loadProjectDialog
        title: qsTr("Select Projectfolder")
        selectExisting: true
        selectMultiple: false
        selectFolder: true
        onFolderChanged: console.log(folder)
        onAccepted:
        {
            if (!configurator.isFolderEmpty(folder))
            {
                yacApp.badMessage(qsTr("Please select an empty folder for your new project."), null,
                                  function()
                                  {
                                      loadProjectDialog.open()
                                  }
                                  )
                return
            }
        }

//        nameFilters: [ "yacApp-Project-Files (*.yacapp)" ]
//        onFileUrlChanged:
//        {
//            yacApp.loadNewProject(fileUrl)
//            configurator.lastProjectFilename = fileUrl
//            configurator.lastProjectName = yacApp.globalConfig.projectName
//            configurator.save()
//            newProjectLoaded()
//            startPage.visible = false
//        }
    }
    FileDialog
    {
        id: createProjectDialog
        selectExisting: false
        selectMultiple: false
        nameFilters: [ "yacApp-Project-Files (*.yacapp)" ]
        onFileUrlChanged:
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
            onClicked: loadProjectDialog.open()
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
}
