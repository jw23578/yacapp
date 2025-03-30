import QtQuick 2.0
import QtQuick.Dialogs
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

    FolderDialog
    {
        id: newProjectDialog
        title: qsTr("Select Projectfolder")
        currentFolder: "/home/jw78"
        onAccepted:
        {
            if (!Helper.isFolderEmpty(selectedFolder))
            {
                CPPQMLAppAndConfigurator.badMessage(qsTr("Please select an empty folder for your new project."), null,
                                                    function()
                                                    {
                                                        loadProjectDialog.open()
                                                    }
                                                    )
                return
            }
            projectFolder.text = selectedFolder
        }
    }
}
