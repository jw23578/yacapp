import QtQuick 2.15
import com.yacapp.parsedconfig 1.0
import com.yacapp.globalprojectconfig 1.0
import com.yacapp.headerconfig 1.0
import com.yacapp.contentconfig 1.0
import QtQuick.Dialogs 1.3
import "qrc:/qml/items"
import "qrc:/qml/dialogs"

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
        YACText
        {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("Pages")
            font.bold: true
        }

        Row
        {
            width: parent.width
            spacing: 1
            YACComboBoxWithHeader
            {
                id: fileCombobox
                headerText: qsTr("Files")
                onActivated: editorForm.loadConfig(text)
                model: yacApp.knownFiles
                width: parent.width - addFileButton.width - 1 - deleteCurrentFileButton.width - 1
            }
            YACButton
            {
                y: fileCombobox.comboBox.y
                id: addFileButton
                text: qsTr("Add File")
                onClicked:
                {
                    configurator.addFileSignal(function(fileName)
                    {
                        configurator.addFormFile(fileName)
                    })
                }
            }
            YACButton
            {
                y: fileCombobox.comboBox.y
                id: deleteCurrentFileButton
                text: qsTr("Delete File")
                onClicked: yacApp.badMessage(qsTr("Not yet implemented"), null, null)
            }
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
                spacing: 1

                YACComboBoxWithHeader
                {
                    id: menuefileCombobox
                    currentIndex: find(config.menueFilename)
                    headerText: qsTr("Menuefile")
                    onActivated:
                    {
                        config.menueFilename = text
                        menueFilenameChanged(text)
                    }
                    model: yacApp.knownMenueFiles
                    width: parent.width
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
                        headerText: qsTr("Logo-Width per thousand")
                        intEdit: true
                        text: global.logoWidthPerThousand
                        onDisplayTextChanged: global.logoWidthPerThousand = text
                    }
                    YACLineEditWithHeader
                    {
                        headerText: qsTr("Logo-Offset-X per thousand")
                        intEdit: true
                        text: global.logoOffsetXPerThousand
                        onDisplayTextChanged: global.logoOffsetXPerThousand = text
                    }
                    YACLineEditWithHeader
                    {
                        headerText: qsTr("Logo-Height per thousand")
                        intEdit: true
                        text: global.logoHeightPerThousand
                        onDisplayTextChanged: global.logoHeightPerThousand = text
                    }
                    YACLineEditWithHeader
                    {
                        headerText: qsTr("Logo-Offset-Y per thousand")
                        intEdit: true
                        text: global.logoOffsetYPerThousand
                        onDisplayTextChanged: global.logoOffsetYPerThousand = text
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
                        headerText: qsTr("Height per thousand")
                        doubleEdit: true
                        text: config.header.heightPerThousand
                        onDisplayTextChanged: config.header.heightPerThousand = text
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
                                id: itemColumn
                                property var currentItem: config.content.items[index]
                                property bool tileType: ["tile"].includes(currentItem.type)
                                property bool slidertileType: ["slidertile"].includes(currentItem.type)
                                property bool flipableType: ["flipable"].includes(currentItem.type)
                                property bool animatedimageType: ["animatedimage"].includes(currentItem.type)
                                width: columnItems.width
                                Item
                                {
                                    height: contentItemType.height / 2
                                    width: 1
                                }

                                Row
                                {
                                    YACComboBoxWithHeader
                                    {
                                        id: contentItemType
                                        width: itemColumn.width - removeItem.width
                                        headerText: qsTr("Type")
                                        currentIndex: find(config.content.items[index].type)
                                        onCurrentTextChanged: config.content.items[index].type = currentText
                                        model: config.content.items[index].typeOptions
                                    }
                                    YACButton
                                    {
                                        id: removeItem
                                        text: qsTr("Remove Item")
                                        onClicked: yacApp.yesNoQuestion(qsTr("Remove Item?")
                                                                        , null
                                                                        , function()
                                                                        {
                                                                            config.content.removeItem(index)
                                                                        }
                                                                        , function()
                                                                        {
                                                                        }

                                            )
                                    }
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
                                    id: theText
                                    headerText: qsTr("Text")
                                    text: currentItem.text
                                    onDisplayTextChanged: currentItem.text = text
                                    visible: tileType || slidertileType || flipableType
                                }
                                YACColorDialogWithHeader
                                {
                                    headerText: qsTr("Color")
                                    color: currentItem.color
                                    onAccepted: currentItem.color = color
                                    visible: tileType || slidertileType || flipableType
                                }
                                YACLineEditWithHeader
                                {
                                    id: theFlipableFilename
                                    headerText: qsTr("FlipableFilename")
                                    text: currentItem.flipableFilename
                                    onDisplayTextChanged: currentItem.flipableFilename = text
                                    visible: slidertileType || flipableType
                                }
                                YACLineEditWithHeader
                                {
                                    id: theText2
                                    headerText: qsTr("Text2")
                                    text: currentItem.text2
                                    onDisplayTextChanged: currentItem.text2 = text
                                    visible: slidertileType || flipableType
                                }
                                YACColorDialogWithHeader
                                {
                                    headerText: qsTr("Color2")
                                    color: currentItem.color2
                                    onAccepted: currentItem.color2 = color
                                    visible: slidertileType || flipableType
                                }
                                YACLineEditWithHeader
                                {
                                    id: theFlipableFilename2
                                    headerText: qsTr("FlipableFilename2")
                                    text: currentItem.flipableFilename2
                                    onDisplayTextChanged: currentItem.flipableFilename2 = text
                                    visible: slidertileType || flipableType
                                }

                                YACLineEditWithHeader
                                {
                                    id: theUrl
                                    headerText: qsTr("Url")
                                    text: config.content.items[index].url
                                    onDisplayTextChanged: config.content.items[index].url= text
                                    visible: ["webview", "image"].includes(config.content.items[index].type) || animatedimageType
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
                                    headerText: qsTr("Width per thousand")
                                    doubleEdit: true
                                    text: config.content.items[index].widthPerThousand
                                    onDisplayTextChanged: config.content.items[index].widthPerThousand = text
                                    visible: columnItems.showRow
                                }
                                YACLineEditWithHeader
                                {
                                    headerText: qsTr("Height per thousand")
                                    doubleEdit: true
                                    text: config.content.items[index].heightPerThousand
                                    onDisplayTextChanged: config.content.items[index].heightPerThousand = text
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
                        headerText: qsTr("Height per thousand")
                        doubleEdit: true
                        text: config.footer.heightPerThousand
                        onDisplayTextChanged: config.footer.heightPerThousand = text
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
