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
        configurator.createNewProject(projectName.displayText,
                                      projectFolder.text,
                                      function()
                                      {
                                          close()
                                          created()
                                      },
                                      projectName,
                                      projectFolder)
    }

    Column
    {
        anchors.centerIn: parent
        width: parent.width * 4 / 5
        YACLineEditWithHeader
        {
            id: projectName
            headerText: qsTr("Projectname")
            focus: true
        }
        YACButtonWithHeader
        {
            id: projectFolder
            headerText: qsTr("Projectfolder")
            emptyText: qsTr("Please select")
            onClicked: newProjectDialog.open()
        }
    }

    FileDialog
    {
        id: newProjectDialog
        title: qsTr("Select Projectfolder")
        selectExisting: false
        selectMultiple: false
        selectFolder: true
        onAccepted:
        {
            if (!Helper.isFolderEmpty(folder))
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
