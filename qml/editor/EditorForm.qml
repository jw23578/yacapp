import QtQuick 2.0
import Qt.labs.platform 1.0
import com.yacapp.parsedconfig 1.0
import com.yacapp.globalprojectconfig 1.0
import "../items"

Rectangle
{
    property ParsedConfig config: null
    property GlobalProjectConfig global: null
    color: "silver"
    FolderDialog {
        id: folderDialog
        onFolderChanged: yacApp.loadNewProject(folder + "/")
    }
    Column
    {
        width: parent.width
        Row
        {
            width: parent.width
            YACButton
            {
                width: parent.width / 2
                text: "Load Project"
                onClicked: folderDialog.open()
            }
            Item
            {
                width: 1
                height: 1
            }
            YACButton
            {
                width: parent.width / 2 - 1
                text: "Save Project"
            }
        }
        YACProjectText
        {
            width: parent.width
            text: "Project: " + global.projectName
        }

        EditorSection
        {
            sectionCaption: "Header"
            YACLineEditWithHeader
            {
                headerText: "Title"
                text: config.header.title
                onDisplayTextChanged: config.header.title = text
            }
            YACLineEditWithHeader
            {
                headerText: "Height"
                text: config.header.height
                onDisplayTextChanged: config.header.height = text
            }
        }
        EditorSection
        {
            sectionCaption: "Content"
        }

        EditorSection
        {
            sectionCaption: "Footer"
        }
    }
}
