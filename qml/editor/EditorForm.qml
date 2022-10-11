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
    signal openOtherProject()
    signal deployProjectClicked()
    Column
    {
        id: topColumn
        width: parent.width
        Row
        {
            width: parent.width
            YACButton
            {
                text: qsTr("Other Project")
                onClicked: openOtherProject()
            }

            spacing: 1
            YACButton
            {
                width: parent.width / 2 - 1
                text: qsTr("Save Project")
                onClicked: yacApp.saveCurrentProject()
            }
            YACButton
            {
                text: qsTr("Deploy Project")
                onClicked: deployProjectClicked()
            }
        }
        YACProjectText
        {
            width: parent.width
            text: qsTr("Project: ") + global.projectName
        }
        YACComboBoxWithHeader
        {
            headerText: qsTr("Files")
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
                    headerText: qsTr("Menuefilename")
                    text: config.menueFilename
                    onDisplayTextChanged:
                    {
                        config.menueFilename = text
                        menueFilenameChanged(text)
                    }
                }
                EditorSection
                {
                    sectionCaption: qsTr("Logo")
                    YACLineEditWithHeader
                    {
                        headerText: qsTr("LogoUrl")
                        text: global.logoUrl
                        onDisplayTextChanged: global.logoUrl = text
                    }
                    YACLineEditWithHeader
                    {
                        headerText: qsTr("Logo-Width")
                        doubleEdit: true
                        text: global.logoWidth
                        onDisplayTextChanged: global.logoWidth = text
                    }
                    YACLineEditWithHeader
                    {
                        headerText: qsTr("Logo-Offset-X")
                        doubleEdit: true
                        text: global.logoOffsetX
                        onDisplayTextChanged: global.logoOffsetX = text
                    }
                    YACLineEditWithHeader
                    {
                        headerText: qsTr("Logo-Height")
                        doubleEdit: true
                        text: global.logoHeight
                        onDisplayTextChanged: global.logoHeight = text
                    }
                    YACLineEditWithHeader
                    {
                        headerText: qsTr("Logo-Offset-Y")
                        doubleEdit: true
                        text: global.logoOffsetY
                        onDisplayTextChanged: global.logoOffsetY = text
                    }
                }

                EditorSection
                {
                    sectionCaption: qsTr("Header")
                    YACLineEditWithHeader
                    {
                        headerText: qsTr("Title")
                        text: config.header.title
                        onDisplayTextChanged: config.header.title = text
                    }
                    YACLineEditWithHeader
                    {
                        headerText: qsTr("Height")
                        doubleEdit: true
                        text: config.header.height
                        onDisplayTextChanged: config.header.height = text
                    }
                    YACColorDialogWithHeader
                    {
                        headerText: qsTr("Color")
                        color: config.header.color
                        onColorChanged: config.header.color = color
                    }
                }
                EditorSection
                {
                    sectionCaption: qsTr("Background")
                    YACColorDialogWithHeader
                    {
                        headerText: qsTr("Color")
                        color: config.background.color
                        onColorChanged: config.background.color = color
                    }
                }

                EditorSection
                {
                    sectionCaption: qsTr("Content")
                    YACSwitch
                    {
                        text: qsTr("Login needed")
                        checked: config.content.loginNeeded
                        onCheckedChanged: config.content.loginNeeded = checked
                    }
                    YACComboBoxWithHeader
                    {
                        id: contentType
                        headerText: qsTr("Type")
                        currentIndex: find(config.content.type)
                        onCurrentTextChanged: config.content.type = currentText
                        model: config.content.typeOptions
                    }
                    YACLineEditWithHeader
                    {
                        showColumn: config.content.type == "grid"
                        intEdit: true
                        headerText: qsTr("Columns")
                        text: config.content.columns
                        onDisplayTextChanged: config.content.columns = text
                    }
                    YACLineEditWithHeader
                    {
                        showColumn: config.content.type == "grid"
                        intEdit: true
                        headerText: qsTr("Rows")
                        text: config.content.rows
                        onDisplayTextChanged: config.content.rows = text
                    }
                    YACLineEditWithHeader
                    {
                        showColumn: config.content.type == "webview"
                        headerText: qsTr("Url")
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
                            model: parent.showColumn || parent.showRow ? config.content.itemCount : 0
                            delegate: Column
                            {
                                width: columnItems.width
                                Item
                                {
                                    height: contentItemType.height / 2
                                    width: 1
                                }

                                YACComboBoxWithHeader
                                {
                                    id: contentItemType
                                    headerText: qsTr("Type")
                                    currentIndex: find(config.content.items[index].type)
                                    onCurrentTextChanged: config.content.items[index].type = currentText
                                    model: config.content.items[index].typeOptions
                                }
                                YACLineEditWithHeader
                                {
                                    id: theTarget
                                    headerText: qsTr("Target")
                                    text: config.content.items[index].target
                                    onDisplayTextChanged: config.content.items[index].target = text
                                }
                                YACLineEditWithHeader
                                {
                                    id: theUrl
                                    headerText: qsTr("Url")
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
                                        headerText: qsTr("Filename")
                                        text: config.content.items[index].filename
                                        onDisplayTextChanged: config.content.items[index].filename = text
                                        width: parent.width - editFileButton.width
                                    }
                                    YACButton
                                    {
                                        id: editFileButton
                                        text: qsTr("Edit")
                                        onClicked: editorForm.loadConfig(config.content.items[index].filename)
                                    }
                                }
                                YACLineEditWithHeader
                                {
                                    headerText: qsTr("Width")
                                    doubleEdit: true
                                    text: config.content.items[index].width
                                    onDisplayTextChanged: config.content.items[index].width = text
                                    visible: columnItems.showRow
                                }
                                YACLineEditWithHeader
                                {
                                    headerText: qsTr("Height")
                                    doubleEdit: true
                                    text: config.content.items[index].height
                                    onDisplayTextChanged: config.content.items[index].height = text
                                    visible: columnItems.showColumn
                                }
                            }
                        }
                        YACButton
                        {
                            visible: parent.showColumn || parent.showRow
                            text: qsTr("Add Item")
                            onClicked: config.content.addItem(-1)
                        }
                    }
                }
                EditorSection
                {
                    sectionCaption: qsTr("Footer")
                    YACLineEditWithHeader
                    {
                        headerText: qsTr("Height")
                        doubleEdit: true
                        text: config.footer.height
                        onDisplayTextChanged: config.footer.height = text
                    }
                    YACColorDialogWithHeader
                    {
                        headerText: qsTr("Color")
                        color: config.footer.color
                        onColorChanged: config.footer.color = color
                    }
                }
            }
        }

    }
}
