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
    YACTwoButtonRow
    {
        leftText: qsTr("OK")
        rightText: qsTr("Abort")
        onRightClicked: newProjectPage.visible = false
    }
}
