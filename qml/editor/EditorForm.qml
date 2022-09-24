import QtQuick 2.0
import com.yacapp.parsedconfig 1.0
import com.yacapp.globalprojectconfig 1.0
import com.yacapp.headerconfig 1.0
import com.yacapp.contentconfig 1.0
import "../items"

Rectangle
{
    id: editorForm
    property ParsedConfig config: null
    property GlobalProjectConfig global: null
    signal loadConfig(string filename)
    signal menueFilenameChanged(string filename)
    Column
    {
        id: topColumn
        width: parent.width
        Row
        {
            width: parent.width
            spacing: 1
            YACButton
            {
                width: parent.width / 2 - 1
                text: "Save Project"
                onClicked: yacApp.saveCurrentProject()
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
    }
    Column
    {
        anchors.top: topColumn.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        Rectangle
        {
            width: parent.width
            height: 1
            color: "black"
        }

        Flickable
        {
            clip: true
            width: parent.width
            height: parent.height
            contentHeight: editorSections.height
            Column
            {
                id: editorSections
                width: parent.width
                spacing: 10
                YACLineEditWithHeader
                {
                    headerText: "Menuefilename"
                    text: config.menueFilename
                    onDisplayTextChanged:
                    {
                        config.menueFilename = text
                        menueFilenameChanged(text)
                    }
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
                        headerText: "Color" + config.header.color
                        color: config.header.color
                        onColorChanged: config.header.color = color
                    }
                }
                EditorSection
                {
                    sectionCaption: "Background"
                    YACColorDialogWithHeader
                    {
                        headerText: "Color"
                        color: config.background.color
                        onColorChanged: config.background.color = color
                    }
                }

                EditorSection
                {
                    sectionCaption: "Content"
                    YACComboBoxWithHeader
                    {
                        id: contentType
                        headerText: "Type"
                        currentIndex: find(config.content.type)
                        onCurrentTextChanged: config.content.type = currentText
                        model: config.content.typeOptions
                    }
                    YACLineEditWithHeader
                    {
                        showColumn: config.content.type == "grid"
                        intEdit: true
                        headerText: "Columns"
                        text: config.content.columns
                        onDisplayTextChanged: config.content.columns = text
                    }
                    YACLineEditWithHeader
                    {
                        showColumn: config.content.type == "grid"
                        intEdit: true
                        headerText: "Rows"
                        text: config.content.rows
                        onDisplayTextChanged: config.content.rows = text
                    }
                    YACLineEditWithHeader
                    {
                        showColumn: config.content.type == "webview"
                        headerText: "Url"
                        text: config.content.url
                        onDisplayTextChanged: config.content.url = text
                    }
                    Column
                    {
                        id: columnItems
                        width: parent.width
                        property bool showRow: ["row", "grid"].includes(config.content.type)
                        property bool showColumn: ["column", "grid"].includes(config.content.type)
                        Repeater
                        {
                            model: config.content.itemCount
                            delegate: Column
                            {
                                width: columnItems.width
                                YACComboBoxWithHeader
                                {
                                    id: contentItemType
                                    headerText: "Type"
                                    currentIndex: find(config.content.items[index].type)
                                    onCurrentTextChanged: config.content.items[index].type = currentText
                                    model: config.content.items[index].typeOptions
                                }
                                YACLineEditWithHeader
                                {
                                    id: theTarget
                                    headerText: "Target"
                                    text: config.content.items[index].target
                                    onDisplayTextChanged: config.content.items[index].target = text
                                }
                                YACLineEditWithHeader
                                {
                                    id: theUrl
                                    headerText: "Url"
                                    text: config.content.items[index].url
                                    onDisplayTextChanged: config.content.items[index].url= text
                                    visible: ["webview", "image"].includes(config.content.items[index].type)
                                }
                                Row
                                {
                                    visible: (columnItems.showColumn || columnItems.showRow) && config.content.items[index].type == "file"
                                    width: parent.width
                                    YACLineEditWithHeader
                                    {
                                        id: theFilename
                                        headerText: "Filename"
                                        text: config.content.items[index].filename
                                        onDisplayTextChanged: config.content.items[index].filename = text
                                        width: parent.width - editFileButton.width
                                    }
                                    YACButton
                                    {
                                        id: editFileButton
                                        text: "Edit"
                                        onClicked: editorForm.loadConfig(config.content.items[index].filename)
                                    }
                                }
                                YACLineEditWithHeader
                                {
                                    headerText: "Width"
                                    doubleEdit: true
                                    text: config.content.items[index].width
                                    onDisplayTextChanged: config.content.items[index].width = text
                                    visible: columnItems.showRow
                                }
                                YACLineEditWithHeader
                                {
                                    headerText: "Height"
                                    doubleEdit: true
                                    text: config.content.items[index].height
                                    onDisplayTextChanged: config.content.items[index].height = text
                                    visible: columnItems.showColumn
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

    }
}
