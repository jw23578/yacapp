import QtQuick 2.0
import Qt.labs.platform 1.0
import com.yacapp.parsedconfig 1.0
import com.yacapp.globalprojectconfig 1.0
import "../items"

Rectangle
{
    id: editorForm
    property ParsedConfig config: null
    property GlobalProjectConfig global: null
    signal loadConfig(string filename)
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
        YACComboBoxWithHeader
        {
            headerText: "Files"
            onActivated: editorForm.loadConfig(text)
            model: yacApp.knownFiles
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
                doubleEdit: true
                text: config.header.height
                onDisplayTextChanged: config.header.height = text
            }
            YACColorDialogWithHeader
            {
                headerText: "Color"
                color: config.header.color
                onColorChanged: config.header.color = color
            }
        }
        EditorSection
        {
            sectionCaption: "Content"
            YACComboBoxWithHeader
            {
                id: menueType
                headerText: "Type"
                currentIndex: find(config.content.type)
                onCurrentTextChanged: config.content.type = currentText
                model: config.content.typeOptions
            }
            YACLineEditWithHeader
            {
                visible: menueType.currentText == "webview"
                headerText: "Url"
                text: config.content.url
                onDisplayTextChanged: config.content.url = text
            }
            Column
            {
                id: columnItems
                visible: menueType.currentText == "column"
                width: parent.width
                Repeater
                {
                    model: config.content.itemCount
                    delegate: Column
                    {
                        width: columnItems.width
                        YACButton
                        {
                            text: "Edit"
                            onClicked: editorForm.loadConfig(config.content.items[index].filename)
                        }
                        YACLineEditWithHeader
                        {
                            id: theFilename
                            headerText: "Filename"
                            text: config.content.items[index].filename
                            onDisplayTextChanged: config.content.items[index].filename = text
                        }
                        YACLineEditWithHeader
                        {
                            headerText: "Height"
                            doubleEdit: true
                            text: config.content.items[index].height
                            onDisplayTextChanged: config.content.items[index].height = text
                        }
                    }
                }
            }
        }

        EditorSection
        {
            sectionCaption: "Footer"
            YACLineEditWithHeader
            {
                headerText: "Height"
                doubleEdit: true
                text: config.footer.height
                onDisplayTextChanged: config.footer.height = text
            }
            YACColorDialogWithHeader
            {
                headerText: "Color"
                color: config.footer.color
                onColorChanged: config.footer.color = color
            }
        }
    }
}
