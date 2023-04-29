import QtQuick 2.0
import QtQuick.Dialogs 1.3
import "qrc:/qml/items"
import "qrc:/qml/dialogs"

DialogBase
{
    id: newProjectPage
    signal created()
    onAbortClicked: close()
    onOkClicked:
    {
        if (projectName.displayText == "")
        {
            CPPQMLAppAndConfigurator.badMessage(qsTr("Please enter the Projectname"), projectName, null)
            return
        }
        if (!configurator.isFolderEmpty(newProjectDialog.folder))
        {
            CPPQMLAppAndConfigurator.badMessage(qsTr("Please select an empty folder for your new project."), null, null)
            return
        }
        configurator.createNewProject(projectName.displayText,
                                      projectFolder.text)
        close()
        created()
    }

    Column
    {
        anchors.centerIn: parent
        width: parent.width * 4 / 5
        YACLineEditWithHeader
        {
            id: projectName
            headerText: qsTr("Projectname")
        }
        YACButtonWithHeader
        {
            id: projectFolder
            headerText: qsTr("Projectfolder")
            text: qsTr("Please select")
            onClicked: newProjectDialog.open()
        }
    }

    FileDialog
    {
        id: newProjectDialog
        title: qsTr("Select Projectfolder")
        selectExisting: true
        selectMultiple: false
        selectFolder: true
        onAccepted:
        {
            if (!configurator.isFolderEmpty(folder))
            {
                CPPQMLAppAndConfigurator.badMessage(qsTr("Please select an empty folder for your new project."), null,
                                  function()
                                  {
                                      loadProjectDialog.open()
                                  }
                                  )
                return
            }
            projectFolder.text = folder
        }
    }
}
