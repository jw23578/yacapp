import QtQuick 2.0
import QtQuick.Dialogs 1.3
import "qrc:/qml/items"

Rectangle
{
    id: newProjectPage
    width: parent.width * 4 / 5
    height: parent.height * 4 / 5
    color: "white"
    anchors.centerIn: parent
    Rectangle
    {
        z: -1
        anchors.centerIn: parent
        width: parent.parent.width
        height: parent.parent.height
        color: "black"
        opacity: 0.5
        MouseArea
        {
            anchors.fill: parent
        }
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
            headerText: qsTr("Projectfolger")
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
                yacApp.badMessage(qsTr("Please select an empty folder for your new project."), null,
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
    YACTwoButtonRow
    {
        leftText: qsTr("OK")
        rightText: qsTr("Abort")
        onRightClicked: newProjectPage.visible = false
        onLeftClicked:
        {
            if (projectName.displayText == "")
            {
                yacApp.badMessage(qsTr("Please enter the Projectname"), projectName, null)
                return
            }
            if (!configurator.isFolderEmpty(newProjectDialog.folder))
            {
                yacApp.badMessage(qsTr("Please select an empty folder for your new project."), null, null)
                return
            }
            newProjectPage.visible = false
        }
    }
}
